cc_binary(name = 'email',
          srcs = [
                 'addr_parse.c',
                 'conf.c',
                 'email.c',
                 'execgpg.c',
                 'mimeutils.c',
                 'progress_bar.c',
                 'sig_file.c',
                 'addy_book.c',
                 'error.c',
                 'file_io.c',
                 'message.c',
                 'processmail.c',
                 'remotesmtp.c',
                 'smtpcommands.c',
                 'utils.c',
         ],
         deps = [
                '//third_part/email/dlib:dlib',
                ],
         libs=[],
         cflags = [
                  '-DEMAIL_VERSION="3.1.3"',
                  '-DCOMPILE_DATE="10/31/2010-01:13:35PM_EDT"',
                  '-DHAVE_CONFIG_H',
                  '-DHAVE_LIBSSL',
#                  '-DEMAIL_DIR="/home/guoliqiang/git-coding/coding/third_part/email//etc/email"'
         ]
         )

