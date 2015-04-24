TESTS = register

check_PROGRAMS = register

register_SOURCES = register.c
register_CFLAGS = -Wall $(SOFIA_CFLAGS) $(CHECK_CFLAGS)
register_LDADD = -l$(top_builddir)/src/evsip_mem.o $(SOFIA_LIBS) $(CHECK_LIBS)
