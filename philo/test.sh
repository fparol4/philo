for i in $(seq 1 20); do
  	./philo 5 610 200 200 7 > "test_$i.log" &
  done
wait
