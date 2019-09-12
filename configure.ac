AC_INIT([ABMSBenchmarkFLAME], [1.0])

AC_PREFIX_DEFAULT([.])
AC_PROG_CC([gcc cc])
AC_LANG([C])

#LIBSROOTDIR

function test_libsrootdir {
 AC_MSG_NOTICE([checking for libs root directory])
  if test "x$LIBSROOT_DIR" == "x" ; then
    AC_MSG_WARN([libs root library directory not specified])
  fi
}

############################################################
#                         ARGS                             #
############################################################

                               
# TAU
AC_ARG_WITH(tau,
            [AS_HELP_STRING([--with-tau],[TAU compilation])],
[WITH_TAU="yes";],)

# libsroot dir
AC_ARG_WITH(libsroot-libdir,
            [AS_HELP_STRING([--with-libsroot-libdir],[LIBSROOT library directory])],
[LIBSROOT_DIR="${withval}/";],)



############################################################
#                        FLAGS                             #
############################################################

# --------------------- TAU -------------------------- #
if test "x$WITH_TAU" != "x" ; then
	CC="tau_cc.sh"
	TAU_CFLAGS="-tau_makefile=~/sfw/tau-2.26.3/x86_64/lib/Makefile.tau-mpi-pdt -tau_options=-optCompInst"
else
	CC="mpicc"
fi


############################################################
#                      CHECKING                            #
############################################################


# Checking headers

#AC_CHECK_HEADERS([${LIBSROOT_DIR}include/fftw3.h ], [], [test_libsrootdir ; AC_MSG_ERROR([Cannot XXXXX find fftw3 library include files.])],
#	[// Do not use default includes])


# Checking libraries:

#AC_CHECK_LIB([fftw3],fftw_plan_dft_r2c, [], [test_libsrootdir ; AC_MSG_ERROR([FFTW3 library not found])])

#checking for tau_cxx.sh

AS_IF([ test "$WITH_TAU" = "yes" ],
[AC_PATH_PROG(TAU_CHECK, tau_cc.sh, ["no"])], [])

AS_IF([ test "$WITH_TAU" = "yes"   && test "$TAU_CHECK" = "no"],
	[AC_MSG_ERROR([tau_cc file not found])], [])
  

AC_SUBST([CC], ["${CC}"])
AC_SUBST([TAU_CFLAGS], ["${TAU_CFLAGS}"])
AC_SUBST([LIBSROOT_DIR], ["${LIBSROOT_DIR}"])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT()

#./configure --with-tau --with-libsroot-libdir=/home/caos/amoreno/sfw/

