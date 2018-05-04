/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "H5Utils.h"
#include <iostream>
#include <stdio.h>


namespace libqch5 {

hid_t openGroup(hid_t parent, char const* group)
{
   hid_t gid = H5Gopen(parent, group, H5P_DEFAULT);
   if (gid < 0) gid = H5Gcreate(parent, group, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   return gid;
}


/// Define operator data structure type for H5Literate callback.
/// During recursive iteration, these structures will form a
/// linked list that can be searched for duplicate groups,
/// preventing infinite recursion.
struct opdata {
    unsigned        recurs;         // Recursion level.  0=root
    struct opdata   *prev;          // Pointer to previous opdata
    haddr_t         addr;           // Group address
};


/// Operator function to be called by H5Literate.
herr_t op_func (hid_t loc_id, const char *name, const H5L_info_t *info,
            void *operator_data);


/// Function to check for duplicate groups in a path.
int group_check (struct opdata *od, haddr_t target_addr);


void listGroup(hid_t gid)
{
    herr_t          status;
    H5O_info_t      infobuf;
    struct opdata   od;

    status = H5Oget_info (gid, &infobuf);
    od.recurs = 0;
    od.prev = NULL;
    od.addr = infobuf.addr;

    // Print the root group and formatting, begin iteration.
    std::cout << "/ {" << std::endl;
    status = H5Literate (gid, H5_INDEX_NAME, H5_ITER_NATIVE, NULL, op_func, (void*) &od);
    std::cout << "}" << std::endl;
}


/************************************************************

  Operator function.  This function prints the name and type
  of the object passed to it.  If the object is a group, it
  is first checked against other groups in its path using
  the group_check function, then if it is not a duplicate,
  H5Literate is called for that group.  This guarantees that
  the program will not enter infinite recursion due to a
  circular path in the file.

 ************************************************************/
herr_t op_func (hid_t loc_id, const char *name, const H5L_info_t *info,
            void *operator_data)
{
    herr_t          status, return_val = 0;
    H5O_info_t      infobuf;
    struct opdata   *od = (struct opdata *) operator_data;
                                /* Type conversion */
    unsigned        spaces = 2*(od->recurs+1);
                                /* Number of whitespaces to prepend
                                   to output */

    /*
     * Get type of the object and display its name and type.
     * The name of the object is passed to this function by
     * the Library.
     */
    status = H5Oget_info_by_name (loc_id, name, &infobuf, H5P_DEFAULT);
    printf ("%*s", spaces, "");     /* Format output */
    switch (infobuf.type) {
        case H5O_TYPE_GROUP:
            std::cout << "Group: " <<  name << " {\n";

            /*
             * Check group address against linked list of operator
             * data structures.  We will always run the check, as the
             * reference count cannot be relied upon if there are
             * symbolic links, and H5Oget_info_by_name always follows
             * symbolic links.  Alternatively we could use H5Lget_info
             * and never recurse on groups discovered by symbolic
             * links, however it could still fail if an object's
             * reference count was manually manipulated with
             * H5Odecr_refcount.
             */
            if ( group_check (od, infobuf.addr) ) {
                printf ("%*s  Warning: Loop detected!\n", spaces, "");
            }
            else {

                /*
                 * Initialize new operator data structure and
                 * begin recursive iteration on the discovered
                 * group.  The new opdata structure is given a
                 * pointer to the current one.
                 */
                struct opdata nextod;
                nextod.recurs = od->recurs + 1;
                nextod.prev = od;
                nextod.addr = infobuf.addr;
                return_val = H5Literate_by_name (loc_id, name, H5_INDEX_NAME,
                            H5_ITER_NATIVE, NULL, op_func, (void *) &nextod,
                            H5P_DEFAULT);
            }
            printf ("%*s}\n", spaces, "");
            break;
        case H5O_TYPE_DATASET:
            printf ("Dataset: %s\n", name);
            break;
        case H5O_TYPE_NAMED_DATATYPE:
            printf ("Datatype: %s\n", name);
            break;
        default:
            printf ( "Unknown: %s\n", name);
    }

    return return_val;
}


/************************************************************

  This function recursively searches the linked list of
  opdata structures for one whose address matches
  target_addr.  Returns 1 if a match is found, and 0
  otherwise.

 ************************************************************/
int group_check (struct opdata *od, haddr_t target_addr)
{
    if (od->addr == target_addr)
        return 1;       /* Addresses match */
    else if (!od->recurs)
        return 0;       /* Root group reached with no matches */
    else
        return group_check (od->prev, target_addr);
                        /* Recursively examine the next node */
}

} // end namespace
