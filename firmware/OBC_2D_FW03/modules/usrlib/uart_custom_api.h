#define uart0_open(OFLAG)	uart_open(0,OFLAG)
#define uart0_close()		uart_close(0 )
#define uart0_read(BUF,N)	uart_read(0,BUF,N)
#define uart0_write(BUF,N)	uart_write(0,BUF,N)
#define uart0_sync()		uart_sync(0 )
#define uart0_init(P_CFG)	uart_init(0,P_CFG)
#define uart0_getc()		uart_getc(0 )
#define uart0_putc(C)		uart_putc(0,C)
#define uart0_peek()		uart_peek(0 )
#define uart0_find(C)		uart_find(0,C)
#define uart0_print(P_STR)	uart_print(0,P_STR)
#define uart0_available()	uart_available(0 )
