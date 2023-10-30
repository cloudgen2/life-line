#!/bin/sh

# @author Cloudgen Wong
# @date 2023-06-07
build_main() {
    FNAME=life-line
    TARGET_DIR=target
    TARGET=${TARGET_DIR}/${FNAME}

    # create the target directory if it doesn't exist
    mkdir -p ${TARGET_DIR}

    # clean the target folder for any previous compilation
    rm -rf ${TARGET}

    gcc -Wall -Werror \
        src/check-tunnel.c \
        src/copy-folder.c \
        src/copy-if-not-exists.c \
        src/display-signal-message.c \
        src/fix-docroot.c \
        src/get-file-permission.c \
        src/handle-exit.c \
        src/life-line.c \
        src/log-message.c \
        src/main.c \
        src/make-directory.c \
        src/remove-old-log.c \
        src/set-file-permission.c \
        src/sync-data-folder.c \
        src/sync-key.c \
        -o ${TARGET}

    if [ $? -ne 0 ]; then
        echo "Build failed. Please check the error messages above."
        exit 1
    fi

    # Make the binary executable
    chmod +x ${TARGET}

    if [ "$1" = "install" ]; then
        cp ${TARGET} /usr/bin/
        echo "Build successful! Please run '${FNAME}'"
    elif [ "$1" = clean ]; then
        rm -rf ${TARGET_DIR}/*
        echo "Folder ${TARGET_DIR} has been cleared!"
    elif [ "$1" = "test" ]; then
        # Set the necessary variables
        tests/test.sh ${TARGET} tests/test-cases.txt
    else
        echo "Build successful! Please run '${TARGET}'"
    fi
}

build_main "$1"