int checkline(int *address, int value){
	int count = 0;
	int temp = 0;
	for (int i=0; i<6; i++){
		if (address[5 + i] == value){
			temp = 0;
			count +=1;
		}
		else if (address[5 + i] == 0){
			temp +=1;			
		}
		else{
			break;
		}
		
		if (temp ==2){
			break;
		}
	}
	temp = 0;
	for (int i=0; i<6; i++){
		if (address[5 - i] == value){
			temp = 0;
			count +=1;
		}
		else if (address[5 - i] == 0){
			temp +=1;			
		}
		else{
			break;
		}
		
		if (temp ==2){
			break;
		}
	}
	return count;
}
void getline(int* state, int* line, int num, int action){
	int interval, index;
	for (int i = 0; i < 4; i++){
		if (i ==0){
			interval = 1;
		}
		else{
			interval = num-2 + i;
		}
		for (int j=0; j<11; j++){
			index = action + (j - 5) * interval;
			if ((index < 0) || (index > (num*num -1))){
				line[13 * i +j] = 2;
			}
			else{
				line[13 * i +j] = state[index];
			}
		}
		line[13 * i + 11] = checkline(&line[12 * i], 1);
		line[13 * i + 12] = checkline(&line[12 * i], -1);
	}
}
