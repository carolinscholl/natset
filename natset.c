#include "natset.h"
#include <stdlib.h>	 


// initialise a set
ns_status_t ns_init(natset_t *set, unsigned int largest){

	unsigned int size = largest/8 +1; 

	set->array_size = size; 
	set->bitarray = calloc(size, sizeof(uint8_t));
	// calloc allocates memory and initiliases it with zeros

	// Just to be safe we set everything to zero manually 
	for(unsigned int i=0;i<size; i++){
		set->bitarray[i]=0; 
	}

	// calloc returns pointer to allocated memory. If unsuccessful it returns NULL
	if(set->bitarray == NULL) return NSS_ERROR; 
	else return NSS_OK; 			
}


// delete set and free memory
void  ns_free(natset_t *set){

	free(set->bitarray);
	set->bitarray = NULL; 
	// After freeing memory, pointer gets assigned symbolic value NULL
}


// add element to set
ns_status_t ns_add(natset_t *set, unsigned int element){

	if(set->bitarray == NULL) return NSS_ERROR; 

	if ((element/8 +1) > set->array_size)
	{
		unsigned int newsize = element/8 +1; 
		unsigned int oldsize = set->array_size; 
		set->bitarray = realloc(set->bitarray, newsize); 				
			for(unsigned int i=oldsize; i<newsize; i++){	
				set->bitarray[i]=0; 
			}
	}

	// Get position of element in bit array
	unsigned int fieldindex = element/8;

	// get bit position
	unsigned int bitposition = element%8 +1;

	unsigned int decimalvalue=1;
	for(unsigned int i = 0; i<bitposition; i++){
		decimalvalue = decimalvalue*2;
	} // loop computes 2^element

	set->bitarray[fieldindex] += decimalvalue; 


	if(set->bitarray == NULL) return NSS_ERROR; 
	else return NSS_OK;

}


// remove element from set
ns_status_t ns_remove(natset_t *set, unsigned int element){

	if(set->bitarray == NULL) return NSS_ERROR; 

	// If element is outside of initialised range, it is not in the set anyways: do nothing and return
	if ((element/8 +1) > set->array_size){		
	return NSS_OK; 									
	}

	// find index of array where to-be-removed element is
	unsigned int fieldindex = element/8;

	// get bit position 
	unsigned int bitposition = element%8 +1;

	// compute decimal value of that position (pow)
	unsigned int decimalvalue=1;
	
	for(unsigned int i = 0; i<bitposition; i++){
		decimalvalue=decimalvalue*2;
	} 

	// bit wise comparison
	if (set->bitarray[fieldindex] & decimalvalue){ 
		// if it is in there, remove it 
		set-> bitarray[fieldindex]-= decimalvalue;}

	// 	else do nothing

	if(set->bitarray == NULL) return NSS_ERROR; 
	else return NSS_OK;

}


// put the intersection of set1 and set2 in set1
ns_status_t ns_cut(natset_t *set1, natset_t *set2){
	// intersection contains all elements that are in both set1 and set2

	if(set1->bitarray == NULL || set2->bitarray == NULL) return NSS_ERROR; 

	unsigned int numbertestbits;
	unsigned int fieldindex;
	unsigned int bitposition;
	unsigned int decimalvalue; 

	// if bitarray of set2 is larger or same size as set1, check until the end of set1
	if(set1->array_size <= set2->array_size){
		numbertestbits = set1->array_size*8;

		for(unsigned int i = 0; i<numbertestbits;i++){
		fieldindex = i/8;
		bitposition = i%8+1;
		decimalvalue=1; 
			for(unsigned int j = 0; j<bitposition; j++){
				decimalvalue=decimalvalue*2;
			}
		
			if (((set1->bitarray[fieldindex] & decimalvalue) && (set2->bitarray[fieldindex] & decimalvalue)) == 0){ // if not in both sets
				if (set1->bitarray[fieldindex] & decimalvalue){  	// if in set1
				set1->bitarray[fieldindex] -= decimalvalue; 	   	// delete it from set1
				}												// if not do nothing 
			}  											
		}
	} 

	// If bitarray of set2 is smaller than set1, check until the end of set2
	if(set2->array_size < set1->array_size){
		numbertestbits = set2->array_size*8;

		for(unsigned int i = 0; i<numbertestbits;i++){
			fieldindex = i/8;
			bitposition = i%8+1;
			decimalvalue=1; 
			for(unsigned int j = 0; j<bitposition; j++){
				decimalvalue=decimalvalue*2;
			}

			if (((set1->bitarray[fieldindex] & decimalvalue) && (set2->bitarray[fieldindex] & decimalvalue)) == 0){ // if not in both sets
				if (set1->bitarray[fieldindex] & decimalvalue){  	// if in set1
				set1->bitarray[fieldindex] -= decimalvalue; 	   	// delete it
				}												// if not do nothing
			}
		}

		// now the additional bits of set1 have to be set to zero
		unsigned int additional = set1->array_size - set2->array_size; 	
		for(unsigned int i = additional; i<set1->array_size; i++){
			set1->bitarray[i] =0; 
		}
	}

	if(set1->bitarray == NULL || set2->bitarray == NULL) return NSS_ERROR; 
	else return NSS_OK;
}
	

// put the union of set1 and set2 in set1
ns_status_t ns_join(natset_t *set1, natset_t *set2){
// union contains all elements that are in s1 or s2 

	if(set1->bitarray == NULL || set2->bitarray == NULL) return NSS_ERROR; 

	unsigned int numbertestbits;
	unsigned int fieldindex;
	unsigned int bitposition;
	unsigned int decimalvalue; 

// If bit array of set2 is larger or the same size as set1, add the missing bytes to set1 

	if(set1->array_size <= set2->array_size){
		numbertestbits = set2->array_size*8;

		set1->bitarray = realloc(set1->bitarray, set2->array_size); 				

		for(unsigned int i = 0; i<numbertestbits;i++){
		fieldindex = i/8;
		bitposition = i%8+1;
		decimalvalue=1; 
			for(unsigned int j = 0; j<bitposition; j++){
				decimalvalue=decimalvalue*2;
			}
		
			if (((set1->bitarray[fieldindex] & decimalvalue) || (set2->bitarray[fieldindex] & decimalvalue)) == 0){ // if in at least one of the sets
				if ((set1->bitarray[fieldindex] & decimalvalue) == 0){  	// if not yet in set1
				set1->bitarray[fieldindex] += decimalvalue; 	   			// set the bit to 1
				}														// if not do nothing
			}  											
		}
	} 

/* If bit array of set2 is smaller than set1, check until the end of set1
	In this case no new memory has to be allocated because all bits that 1 in set1 belong to union*/
	
	if(set2->array_size < set1->array_size){
		numbertestbits = set1->array_size*8;

		for(unsigned int i = 0; i<numbertestbits;i++){
			fieldindex = i/8;
			bitposition = i%8+1;
			decimalvalue=1; 
			for(unsigned int j = 0; j<bitposition; j++){
				decimalvalue=decimalvalue*2;
			}

			if (((set1->bitarray[fieldindex] & decimalvalue) || (set2->bitarray[fieldindex] & decimalvalue)) == 1){ // if in at least one of the sets
				if ((set1->bitarray[fieldindex] & decimalvalue) == 0){  	// if not yet in set 1
				set1->bitarray[fieldindex] += decimalvalue; 	   			// set the bit to 1 				
				}														// if not do nothing
			}
		}	
	}

	if(set1->bitarray == NULL || set2->bitarray == NULL) return NSS_ERROR; 
	else return NSS_OK;
}




