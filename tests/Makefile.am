check_PROGRAMS = check_evsip
# Get All object for EvSIP and remove the main object only.
EVSIP_OBJS = $(subst $(top_builddir)/src/evsip-evsip_main.o,,$(wildcard $(top_builddir)/src/*.o))

check_evsip_SOURCES = test_mem.c
check_evsip_CFLAGS = -Wall $(SOFIA_CFLAGS) $(CHECK_CFLAGS)
check_evsip_LDADD = $(EVSIP_OBJS) $(SOFIA_LIBS) $(CHECK_LIBS)

TESTS = $(check_PROGRAMS)
