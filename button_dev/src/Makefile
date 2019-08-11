OBJ = buttons

CC = arm-none-linux-gnueabi-gcc

$(OBJ):*.c
	$(CC) $^ -o $@
	
cp:
	cp -i $(OBJ) /home/lsq/tftpboot	
	
clean:
	rm -rf $(OBJ)