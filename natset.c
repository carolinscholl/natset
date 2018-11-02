#include "natset.h"
#include <stdlib.h>	 
#include <stdio.h>


// Function initialises the structure that the parameter set points to 
// as an empty set that accepts elements between 0 and (at least) largest. 
ns_status_t ns_init(natset_t *set, unsigned int largest){

	// compute how many bytes we need to store values between 0 and largest
	unsigned int n_bytes = largest/8 +1; 

	// set the size
	set->array_size = n_bytes; 

	// allocate the needed bytes
	set->bitarray = calloc(n_bytes, sizeof(uint8_t));

	// Just to be safe we set everything to zero manually 
	for(unsigned int i=0;i<n_bytes; i++){
		set->bitarray[i]=0; 
	}

	// calloc returns pointer to allocated memory. If unsuccessful it returns NULL
	if(set->bitarray == NULL) return NSS_ERROR; 
	else return NSS_OK; 			
}


// delete set and free memory
void  ns_free(natset_t *set){

	free(set->bitarray);
	set->bitarray = NULL; // pointer gets assigned symbolic value NULL
}


// Function adds element parameter to the set. If set is not large enough yet 
// (i.e. does not have enough bytes reserved yet), it first adapts the size of the set. 

ns_status_t ns_add(natset_t *set, unsigned int element){

	// if set does not exist abort
	if(set->bitarray == NULL) return NSS_ERROR; 

	// if largest value is out of the range of the reserved bytes, we need to re-allocate memory
	if ((element/8 +1) > set->array_size)
	{
		unsigned int newsize = element/8 +1; 
		unsigned int oldsize = set->array_size; 
		set->bitarray = realloc(set->bitarray, newsize); 				
			for(unsigned int i=oldsize; i<newsize; i++){	
				set->bitarray[i]=0; 
			}
	}

	// Get index of byte-array that the element needs to be stored in
	unsigned int fieldindex = element/8;
	// get bit position in that byte
	unsigned int bitposition = element%8;

	// compute the decimal value that the element represents; mind that 0 gets the first bit!
	// e.g. 0 -> 0000 0001 -> decimal value 1
	// e.g. 2 -> 0000 0100 -> decimal value 3
	unsigned int decimalvalue=1;
	for(unsigned int i = 0; i<bitposition; i++){
		decimalvalue = decimalvalue*2;

	} // loop computes 2^element

	// add this decimal value to the respective byte
	set->bitarray[fieldindex] += decimalvalue; 

	return NSS_OK;
}


// Function removes the element indicated by the parameter element from the set. 
// If element is not in set, set remains unchanged. 
ns_status_t ns_remove(natset_t *set, unsigned int element){

	// if set does not exist abort
	if(set->bitarray == NULL) return NSS_ERROR; 

	// If element is outside of initialised range, it is not in the set anyways: do nothing and return
	if ((element/8 +1) > set->array_size){		
		return NSS_OK; 									
	}

	// find index of byte array where to-be-removed element is
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

	return NSS_OK;
}


// Function puts the intersection of set1 and set2 in set1
ns_status_t ns_cut(natset_t *set1, natset_t *set2){
	// intersection contains all elements that are in both set1 and set2

	// if one of the sets does not exist, abort
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

	return NSS_OK;
}
	

// Function puts the union of set1 and set2 in set1.
ns_status_t ns_join(natset_t *set1, natset_t *set2){
// union contains all elements that are in s1 or s2 

	// if one of the sets does not exist, abort
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

	// If bit array of set2 is smaller than set1, check until the end of set1.
	// In this case no new memory has to be allocated because all bits that 1 in set1 belong to union.
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

	return NSS_OK;
}


ns_status_t print_set(natset_t *set){

		// if set does not exist, abort
		if(set == NULL){
			printf("Set does not exist.\n");
			return NSS_ERROR;
		}

		// get how many bytes we have to check for elements
		int length_bitarry = set->array_size;
		
		printf("Elements in set: ");

		// for each of these bytes
		for(unsigned int i=0;i<length_bitarry; i++){ 
			// most significant bit of each byte is 128 because 0 gets 1st bit
			int decimalvalue = 128; 
			// save the decimal value that is saved in the array
			int left = set->bitarray[i];
			// iterate over it from the last element
			for(int bit=7; bit>=0; bit--){ 	
				// if the remaining value is larger than the decimal value of the current bit
				if(left >= decimalvalue){
					// bit is in set -> print it
					printf("%d ", bit+i*8);
					// subtract it from remaining value
					left-=decimalvalue;
				}
				// compute value of next bit to the right
				decimalvalue-=decimalvalue/2;
			}
		}
		printf("\n");	

		return NSS_OK;
}

