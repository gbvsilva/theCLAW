// Colisão dos dedos da garra
/*
    	//Ponta direita
    	if(
    	catchPtDir[13] > cube->pos[1]-cube->size &&
    	catchPtDir[13] < cube->pos[1]+cube->size){
    		if(
    		catchPtDir[12] > cube->pos[0]-cube->size &&
    		catchPtDir[12] < cube->pos[0]+cube->size){    		
				if(
				catchPtDir[14] > cube->pos[2]-cube->size-0.01 &&
				catchPtDir[14] < cube->pos[2]-cube->size+0.01){
					cube->pos[2] += 0.01;
				}
				else if(
				catchPtDir[14] > cube->pos[2]+cube->size-0.01 &&
				catchPtDir[14] < cube->pos[2]+cube->size+0.01){
					cube->pos[2] -= 0.01;
				}
			}
			else if(
    		catchPtDir[14] > cube->pos[2]-cube->size &&
    		catchPtDir[14] < cube->pos[2]+cube->size){    		
				if(
				catchPtDir[12] > cube->pos[0]-cube->size-0.01 &&
				catchPtDir[12] < cube->pos[0]-cube->size+0.01){
					cube->pos[0] += 0.01;
				}
				else if(
				catchPtDir[12] > cube->pos[0]+cube->size-0.01 &&
				catchPtDir[12] < cube->pos[0]+cube->size+0.01){
					cube->pos[0] -= 0.01;
				}
			}			
    	}
    	// Ponta esquerda
    	if(
    	catchPtEsq[13] > cube->pos[1]-cube->size &&
    	catchPtEsq[13] < cube->pos[1]+cube->size){
    		if(
    		catchPtEsq[12] > cube->pos[0]-cube->size &&
    		catchPtEsq[12] < cube->pos[0]+cube->size){    		
				if(
				catchPtEsq[14] > cube->pos[2]-cube->size-0.01 &&
				catchPtEsq[14] < cube->pos[2]-cube->size+0.01){
					cube->pos[2] += 0.01;
				}
				else if(
				catchPtEsq[14] > cube->pos[2]+cube->size-0.01 &&
				catchPtEsq[14] < cube->pos[2]+cube->size+0.01){
					cube->pos[2] -= 0.01;
				}
			}
			else if(
    		catchPtEsq[14] > cube->pos[2]-cube->size &&
    		catchPtEsq[14] < cube->pos[2]+cube->size){    		
				if(
				catchPtEsq[12] > cube->pos[0]-cube->size-0.01 &&
				catchPtEsq[12] < cube->pos[0]-cube->size+0.01){
					cube->pos[0] += 0.01;
				}
				else if(
				catchPtEsq[12] > cube->pos[0]+cube->size-0.01 &&
				catchPtEsq[12] < cube->pos[0]+cube->size+0.01){
					cube->pos[0] -= 0.01;
				}
			}			
    	}
    	*/
    	// Ambas
    	/*
    	if(
    	catchPtDir[13] > cube->pos[1]-cube->size &&
    	catchPtDir[13] < cube->pos[1]+cube->size &&
    	catchPtEsq[13] > cube->pos[1]-cube->size &&
    	catchPtEsq[13] < cube->pos[1]+cube->size){
    		if(
    		catchPtDir[12] > cube->pos[0]-cube->size &&
    		catchPtDir[12] < cube->pos[0]+cube->size &&
    		catchPtEsq[12] > cube->pos[0]-cube->size &&
    		catchPtEsq[12] < cube->pos[0]+cube->size){  
				if(
				(catchPtDir[14] > cube->pos[2]-cube->size-0.05 &&
				catchPtDir[14] < cube->pos[2]-cube->size+0.05 &&
				catchPtEsq[14] > cube->pos[2]+cube->size-0.05 &&
				catchPtEsq[14] < cube->pos[2]+cube->size+0.05) ||
				(catchPtEsq[14] > cube->pos[2]-cube->size-0.05 &&
				catchPtEsq[14] < cube->pos[2]-cube->size+0.05 &&
				catchPtDir[14] > cube->pos[2]+cube->size-0.05 &&
				catchPtDir[14] < cube->pos[2]+cube->size+0.05)){
					cube->clr[2] = 0.0;
					caught = true;
				}
				else if(
				(catchPtDir[14] > cube->pos[2]+cube->size-0.05 &&
				catchPtDir[14] < cube->pos[2]+cube->size+0.05 &&
				catchPtEsq[14] > cube->pos[2]-cube->size-0.05 &&
				catchPtEsq[14] < cube->pos[2]-cube->size+0.05) ||
				(catchPtEsq[14] > cube->pos[2]+cube->size-0.05 &&
				catchPtEsq[14] < cube->pos[2]+cube->size+0.05 &&
				catchPtDir[14] > cube->pos[2]-cube->size-0.05 &&
				catchPtDir[14] < cube->pos[2]-cube->size+0.05)){
					cube->clr[2] = 0.0;
					caught = true;
				}
			}
			else if(
    		catchPtDir[14] > cube->pos[2]-cube->size &&
    		catchPtDir[14] < cube->pos[2]+cube->size &&
    		catchPtEsq[14] > cube->pos[2]-cube->size &&
    		catchPtEsq[14] < cube->pos[2]+cube->size){
				if(
				(catchPtDir[12] > cube->pos[0]-cube->size-0.1 &&
				catchPtDir[12] < cube->pos[0]-cube->size+0.1 &&
				catchPtEsq[12] > cube->pos[0]+cube->size-0.1 &&
				catchPtEsq[12] < cube->pos[0]+cube->size+0.1) ||
				(catchPtEsq[12] > cube->pos[0]-cube->size-0.1 &&
				catchPtEsq[12] < cube->pos[0]-cube->size+0.1 &&
				catchPtDir[12] > cube->pos[0]+cube->size-0.1 &&
				catchPtDir[12] < cube->pos[0]+cube->size+0.1)){
					cube->clr[2] = 0.0;
					caught = true;
				}
				else if(
				(catchPtDir[12] > cube->pos[0]+cube->size-0.1 &&
				catchPtDir[12] < cube->pos[0]+cube->size+0.1 &&
				catchPtEsq[12] > cube->pos[0]-cube->size-0.1 &&
				catchPtEsq[12] < cube->pos[0]-cube->size+0.1) ||
				(catchPtEsq[12] > cube->pos[0]+cube->size-0.1 &&
				catchPtEsq[12] < cube->pos[0]+cube->size+0.1 &&
				catchPtDir[12] > cube->pos[0]-cube->size-0.1 &&
				catchPtDir[12] < cube->pos[0]-cube->size+0.1)){
					cube->clr[2] = 0.0;
					caught = true;
				}
			}			
    	}
    	else{
    		cube->clr[0] = 1.0;
    		cube->clr[1] = 1.0;
    		cube->clr[2] = 1.0;
    		caught = false;
    	}*/
