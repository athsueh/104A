# $Id: Makefile,v 1.2 2015-01-20 21:25:38-08 - - $

MKFILE      = Makefile
DEPFILE     = ${MKFILE}.dep
NOINCL      = ci clean spotless
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory

COMPILECPP  = g++ -g -O0 -Wall -Wextra -rdynamic -std=gnu++11
MAKEDEPCPP  = g++ -MM

CPPSOURCE   = auxlib.cpp cppstrtok.cpp main.cpp stringset.cpp
CPPHEADER   = auxlib.h stringset.h
EXECBIN     = yshell
OBJECTS     = ${CPPSOURCE:.cpp=.o}
OTHERS      = ${MKFILE} README PARTNER
ALLSOURCES  = ${CPPHEADER} ${CPPSOURCE} ${OTHERS}
LISTING     = Listing.ps

all : ${EXECBIN}
	- checksource ${ALLSOURCES}

${EXECBIN} : ${OBJECTS}
	${COMPILECPP} -o $@ ${OBJECTS}

%.o : %.cpp
	${COMPILECPP} -c $<

ci : ${ALLSOURCES}
	cid + ${ALLSOURCES}
	- checksource ${ALLSOURCES}

lis : ${ALLSOURCES}
	mkpspdf ${LISTING} ${ALLSOURCES} ${DEPFILE}

clean :
	- rm ${OBJECTS} ${DEPFILE} core ${EXECBIN}.errs

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}

dep : ${CPPSOURCE} ${CPPHEADER}
	@ echo "# ${DEPFILE} created `LC_TIME=C date`" >${DEPFILE}
	${MAKEDEPCPP} ${CPPSOURCE} >>${DEPFILE}

${DEPFILE} : ${MKFILE}
	@ touch ${DEPFILE}
	${GMAKE} dep

again :
	${GMAKE} spotless dep ci all lis

ifeq (${NEEDINCL}, )
include ${DEPFILE}
endif
