#!/bin/sh
 -q --enable=style,performance,portability,information \
           --suppressions-list=./cppcheck-suppress \
           --error-exitcode=1 .
exit $?
