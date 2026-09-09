SOURCE_FILE_V1=foo_1.c
SOURCE_FILE_V2=foo_2.c
MAIN_FILE=main.c
SONAME=libfoo.so.1
REAL_NAME_V1=libfoo.so.1.1
REAL_NAME_V2=libfoo.so.1.2
LINKER_NAME=libfoo.so
OUTPUT_FILE=a.out

v1:
	gcc -shared -fPIC -Wl,-soname,${SONAME} -o ${REAL_NAME_V1} ${SOURCE_FILE_V1}
	ln -sf ${REAL_NAME_V1} ${LINKER_NAME}
	gcc ${MAIN_FILE} -I. -L. -lfoo -o ${OUTPUT_FILE}
	# Same as `ldconfig -n .`, creates a symbolic link
	# ln -sf ${REAL_NAME_V1} ${SONAME}
	ldconfig -n .

v2:
	gcc -shared -fPIC -Wl,-soname,${SONAME} -o ${REAL_NAME_V2} ${SOURCE_FILE_V2}
	# Same as `ldconfig -n .`, creates a symbolic link
	# ln -sf ${REAL_NAME_V2} ${SONAME}
	ldconfig -n .

test:
	#./a.out: error while loading shared libraries: libfoo.so.1: cannot open 
	# shared object file: No such file or directory
	LD_LIBRARY_PATH=. ./${OUTPUT_FILE}

clean:
	rm ${SONAME} ${REAL_NAME_V1} ${REAL_NAME_V2} ${LINKER_NAME} ${OUTPUT_FILE}
