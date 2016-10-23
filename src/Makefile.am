bin_PROGRAMS	=	evsip

evsip_CFLAGS	=	-Wall -fvisibility=hidden -fstack-check -finstrument-functions $(SOFIA_CFLAGS)
evsip_LDADD	=	$(SOFIA_LIBS)

evsip_SOURCES	=	evsip_main.c evsip_sig.c evsip_log.c evsip_glob.c \
			evsip_core.c evsip_config.c evsip_cli.c evsip_cli_cmdlst.c evsip_mem.c \
			evsip_evsofia.c evsip_endp.c evsip_register.c evsip_invite.c 


