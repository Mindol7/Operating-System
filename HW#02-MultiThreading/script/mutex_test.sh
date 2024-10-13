READFILE="/home/minhyuk/OperatingSystem/HW#02-Multi-Threading/HW#02-MultiThreading/utils/FreeBSD9-orig.tar"
PRODUCER_COUNTS=(100)
CONSUMER_COUNTS=(100)
OUTPUT_FILE="/home/minhyuk/OperatingSystem/HW#02-Multi-Threading/HW#02-MultiThreading/src/Mutex/mutex_execution_times.txt"

echo "Execution Time Results" >> $OUTPUT_FILE

for P in "${PRODUCER_COUNTS[@]}"; do
    for C in "${CONSUMER_COUNTS[@]}"; do
        echo "Running with $P producer(s) and $C consumer(s):"

        EXEC_TIME=$(/home/minhyuk/OperatingSystem/HW#02-Multi-Threading/HW#02-MultiThreading/src/Mutex/word_count $READFILE $P $C | grep "Execution Time" | awk '{print $3}')

        printf "Execution Time: %.3f seconds\n" $EXEC_TIME >> $OUTPUT_FILE

        echo "Producers: $P, Consumers: $C" >> $OUTPUT_FILE
        echo "" >> $OUTPUT_FILE
    done
done