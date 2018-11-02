#include "natset.c"
#include <stdio.h>


int main(){
	natset_t s1, s2; // define two sets, s1 and s2

	if(ns_init(&s1,15) == 0)
		{printf("Set 1 created.\n");
	}
	else{
		printf("Error initialising set 1");
	}


	if(ns_init(&s2,31) == 0)
		{printf("Set 2 created.\n");
	}
	else{
		printf("Error initialising set 2");
	}


	ns_add(&s1,3); // add element 3 to s1 => s1={3}
	ns_add(&s1,14); // => s1={3,14}
	ns_add(&s1,0); //  => s1={0,3,14}
	printf("Set 1 - ");
	print_set(&s1); 

	ns_add(&s2,0); //  => s2={0}
	ns_add(&s2,30); // => s2={0,30}
	printf("Set 2 - " );
	print_set(&s2);

	ns_cut(&s1,&s2); // s1 := s1 \ s2 => s1={0}
	printf("Set 1 - ");
	print_set(&s1); 

}