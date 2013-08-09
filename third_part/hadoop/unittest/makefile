all:
	../../../develop/snake/snake.sh t=*
	cp ../../../.snake/dbg/third_part/hadoop/unittest/word_count .
	hadoop fs -rm /usr/local/hadoop/hdfs/bin/word_count
	hadoop fs -put ./word_count /usr/local/hadoop/hdfs/bin/
