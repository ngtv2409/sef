#ifndef SEF_LIMITS_H
#define SEF_LIMITS_H

/* 
 @ SEF constraints
 ! Should not be change because it affects ABI
 ! This is hardcoded once and forever
 ! Only modify if you know what you are doing

TODO: Add functions which uses the heap to fight constraints
*/

/* @ Maximum number of dependencies in IR */
#define SEF_LIM_MAXDEP 16
/* @ Maximum format pipe length */
#define SEF_LIM_MAXPIPE 8
/* @ Maximum intermidiate string length including terminating character
     (the buffer steps in pipeline) 
*/
#define SEF_LIM_MAXINTSTR 256

#endif /* SEF_LIMITS_H */
