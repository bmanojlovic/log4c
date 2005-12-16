#
# Helper script to run the test programs
# compiled into the log4c/tests directory
# eg ./go_test.sh test_stream2
#
export PATH=log4c/log4c:$PATH
log4c/tests/$1
