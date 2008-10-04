#define HTTP_DOS
#define HTTP_DENY		-1
#define HTTP_ALLOW	-2


/* direction */
#define C2OUT 10
#define OUT2C 20
#define OUT2S 30
#define S2OUT 40

/* action */
#define SIN_ACCEPT 1
#define SIN_DENY 2
#define SIN_CONTINUE 3

/* session type */
#define SESSION_E
#define SESSION_P
typedef struct _ctx{
	char *buf;
	int flag;
	int status;
	struct ipq_handle *h;
	gpointer extra;

} packet_ctx;



struct _t {
	char direction;
	char verdicted;
	char action;
	char sessionType;
	char modified;
	int packetLength;
	unsigned char *packet;
};

typedef struct _t sin_packet_info;
