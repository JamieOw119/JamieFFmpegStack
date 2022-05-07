# AVIO test

This program is designed to explore the role of the AVIOContext structure to understand how FFmpeg reads and writes from different URLs/files.

The following are the important member variables in the AVIO structure that I personally think:

```
typedef struct AVIOContext {
    const AVClass *av_class;
    unsigned char *buffer;  /**< Start of the buffer. */
    int buffer_size;        /**< Maximum buffer size */
    unsigned char *buf_ptr; /**< Current position in the buffer */
    unsigned char *buf_end; /**< End of the data, may be less than
                                 buffer+buffer_size if the read function returned
                                 less data than requested, e.g. for streams where
                                 no more data has been received yet. */
    void *opaque;           /**< A private pointer, passed to the read/write/seek/...
                                 functions. */
    int (*read_packet)(void *opaque, uint8_t *buf, int buf_size);
    int (*write_packet)(void *opaque, uint8_t *buf, int buf_size);
    int64_t (*seek)(void *opaque, int64_t offset, int whence);
    int write_flag;         /**< true if open for writing */
    int64_t pos;            /**< position in the file of the current buffer */
    /**
     * Maximum reached position before a backward seek in the write buffer,
     * used keeping track of already written data for a later flush.
     */
    unsigned char *buf_ptr_max;
}
```

The following notes excerpted from the official program detail how AVIO's read and write pointers operate.

```
/*
* The following shows the relationship between buffer, buf_ptr,
* buf_ptr_max, buf_end, buf_size, and pos, when reading and when writing
* (since AVIOContext is used for both):
*
**********************************************************************************
*                                   READING
**********************************************************************************
*
*                            |              buffer_size              |
*                            |---------------------------------------|
*                            |                                       |
*
*                         buffer          buf_ptr       buf_end
*                            +---------------+-----------------------+
*                            |/ / / / / / / /|/ / / / / / /|         |
*  read buffer:              |/ / consumed / | to be read /|         |
*                            |/ / / / / / / /|/ / / / / / /|         |
*                            +---------------+-----------------------+
*
*                                                         pos
*              +-------------------------------------------+-----------------+
*  input file: |                                           |                 |
*              +-------------------------------------------+-----------------+
*
*
**********************************************************************************
*                                   WRITING
**********************************************************************************
*
*                             |          buffer_size                 |
*                             |--------------------------------------|
*                             |                                      |
*
*                                                buf_ptr_max
*                          buffer                 (buf_ptr)       buf_end
*                             +-----------------------+--------------+
*                             |/ / / / / / / / / / / /|              |
*  write buffer:              | / / to be flushed / / |              |
*                             |/ / / / / / / / / / / /|              |
*                             +-----------------------+--------------+
*                               buf_ptr can be in this
*                               due to a backward seek
*
*                            pos
*               +-------------+----------------------------------------------+
*  output file: |             |                                              |
*               +-------------+----------------------------------------------+
*
*/
```

How does AVIOContext bind the file to be read to the buffer? opaque points to the URLContext.

```
typedef struct URLContext {
	const AVClass *av_class; ///< information for av_log(). Set by url_open().
	struct URLProtocol *prot;
	int flags;
	int is_streamed;  /**< true if streamed (no seek possible), default = false */
	int max_packet_size;  /**< if non zero, the stream is packetized with this max packet size */
	void *priv_data;
	char *filename; /**< specified URL */
	int is_connected;
	AVIOInterruptCB interrupt_callback;
} URLContext;
```

There is also a structure URLProtocol in the URLContext structure. Note: Each protocol (rtp, rtmp, file, etc.) corresponds to a URLProtocol.

```
typedef struct URLProtocol {
	const char *name;
	int (*url_open)(URLContext *h, const char *url, int flags);
	int (*url_read)(URLContext *h, unsigned char *buf, int size);
	int (*url_write)(URLContext *h, const unsigned char *buf, int size);
	int64_t (*url_seek)(URLContext *h, int64_t pos, int whence);
	int (*url_close)(URLContext *h);
	struct URLProtocol *next;
	int (*url_read_pause)(URLContext *h, int pause);
	int64_t (*url_read_seek)(URLContext *h, int stream_index,
		int64_t timestamp, int flags);
	int (*url_get_file_handle)(URLContext *h);
	int priv_data_size;
	const AVClass *priv_data_class;
	int flags;
	int (*url_check)(URLContext *h, int mask);
} URLProtocol;
```

Suppose we read and write the file, then the callback function is registered like this：

```
URLProtocol ff_file_protocol = {
    .name                = "file",
    .url_open            = file_open,
    .url_read            = file_read,
    .url_write           = file_write,
    .url_seek            = file_seek,
    .url_close           = file_close,
    .url_get_file_handle = file_get_handle,
    .url_check           = file_check,
};
```

```
static int file_read(URLContext *h, unsigned char *buf, int size)
{
    int fd = (intptr_t) h->priv_data;
    int r = read(fd, buf, size);
    return (-1 == r)?AVERROR(errno):r;
}
 
static int file_write(URLContext *h, const unsigned char *buf, int size)
{
    int fd = (intptr_t) h->priv_data;
    int r = write(fd, buf, size);
    return (-1 == r)?AVERROR(errno):r;
}
 
static int file_get_handle(URLContext *h)
{
    return (intptr_t) h->priv_data;
}
 
static int file_check(URLContext *h, int mask)
{
    struct stat st;
    int ret = stat(h->filename, &st);
    if (ret < 0)
        return AVERROR(errno);
 
    ret |= st.st_mode&S_IRUSR ? mask&AVIO_FLAG_READ  : 0;
    ret |= st.st_mode&S_IWUSR ? mask&AVIO_FLAG_WRITE : 0;
 
    return ret;
}
 
#if CONFIG_FILE_PROTOCOL
 
static int file_open(URLContext *h, const char *filename, int flags)
{
    int access;
    int fd;
 
    av_strstart(filename, "file:", &filename);
 
    if (flags & AVIO_FLAG_WRITE && flags & AVIO_FLAG_READ) {
        access = O_CREAT | O_TRUNC | O_RDWR;
    } else if (flags & AVIO_FLAG_WRITE) {
        access = O_CREAT | O_TRUNC | O_WRONLY;
    } else {
        access = O_RDONLY;
    }
#ifdef O_BINARY
    access |= O_BINARY;
#endif
    fd = open(filename, access, 0666);
    if (fd == -1)
        return AVERROR(errno);
    h->priv_data = (void *) (intptr_t) fd;
    return 0;
}
 
/* XXX: use llseek */
static int64_t file_seek(URLContext *h, int64_t pos, int whence)
{
    int fd = (intptr_t) h->priv_data;
    if (whence == AVSEEK_SIZE) {
        struct stat st;
        int ret = fstat(fd, &st);
        return ret < 0 ? AVERROR(errno) : st.st_size;
    }
    return lseek(fd, pos, whence);
}
 
static int file_close(URLContext *h)
{
    int fd = (intptr_t) h->priv_data;
    return close(fd);
}
```

The same is true for changing transport protocols such as RTMP, UDP, etc.

```
URLProtocol ff_rtmp_protocol = {
    .name                = "rtmp",
    .url_open            = rtmp_open,
    .url_read            = rtmp_read,
    .url_write           = rtmp_write,
    .url_close           = rtmp_close,
    .url_read_pause      = rtmp_read_pause,
    .url_read_seek       = rtmp_read_seek,
    .url_get_file_handle = rtmp_get_file_handle,
    .priv_data_size      = sizeof(RTMP),
    .flags               = URL_PROTOCOL_FLAG_NETWORK,
};
```

```
URLProtocol ff_udp_protocol = {
    .name                = "udp",
    .url_open            = udp_open,
    .url_read            = udp_read,
    .url_write           = udp_write,
    .url_close           = udp_close,
    .url_get_file_handle = udp_get_file_handle,
    .priv_data_size      = sizeof(UDPContext),
    .flags               = URL_PROTOCOL_FLAG_NETWORK,
};
```

Allocate and initialize an AVIOContext for buffered I/O.

```
AVIOContext* avio_alloc_context	(	
    unsigned char * 	buffer,
    int 	buffer_size,
    int 	write_flag,
    void * 	opaque,
    int(*)(void *opaque, uint8_t *buf, int buf_size) 	read_packet,
    int(*)(void *opaque, uint8_t *buf, int buf_size) 	write_packet,
    int64_t(*)(void *opaque, int64_t offset, int whence) 	seek 
)	
```

Finally read a byte from the buffer.

```
int avio_r8(AVIOContext *s)
{
    if (s->buf_ptr >= s->buf_end)
        fill_buffer(s);
    if (s->buf_ptr < s->buf_end)
        return *s->buf_ptr++;
    return 0;
}
```