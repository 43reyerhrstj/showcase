/*
 * The storage engine functions prefixed with `wrapper`, map the corresponding k-v operations 
 * of the k-v DSL to actual B+Tree functions invoked on your B+Tree.
 */
#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include "data_types.h"
#include "query.h"
#include "btree.h"

/*
 * A struct to store the context of your storage engine.
 * You may wish to extend this struct, to not only store your B+Tree, but also store additional state.
 * 
 * 1. You will need to decide how to hold a B+Tree in your storage context. 
 *      Hint, you will need to modify the storage context struct, 
 *      while thinking about memory management and pointers for accessing your B+Tree.
 */
typedef struct storageContext {
    // TODO: you hold a pointer here to find your B+Tree
    Node * root;
} STORAGECXT_t;



/*
 * The following are wrapper functions which are entry points into your storage engine.
 * 
 * You will need to make sure these call your B+Tree to do actual data acccess operations.
 * Your B+Tree will need multiple supporting methods in order to be complete.
 * It is up to you to design: define and declare the additional functions B+t
 */

/*
 *  Get looks for a `targetKey`, and will give back the corresponding key in foundVal, if it exists.
 * 
 *  @Params: 
 *      storageEngine   - the storage engine to operate on
 *      targetKey       - the target key to search for
 *      foundVal        - declared outside the function invocation. should hold any found value.
 * 
 *  Returns:
 *       0 for no result, 
 *       1 if a value exists for `targetKey` in which case `foundVal` is populated
 * 
 */
void wrapperGet(STORAGECXT_t **storageEngine, KEY_t targetKey/*, VAL_t * foundVal*/){
    Get((*storageEngine)->root, targetKey);
}

/*
 *  Put sets a key value pair. 
 *  If a key does not exist previously in the storage engine, it should be inserted.
 *  If the key exists previously, the old value should be overwritten with the newly specified value.
 * 
 *  @Params: 
 *      storageEngine   - the storage engine to operate on
 *      key             - key to add
 *      val             - val to add
 * 
 *  Returns:
 *       0 for failed, 
 *       1 if the k,v pair addition succeeded
 * 
 */
void wrapperPut(STORAGECXT_t **storageEngine, KEY_t key, VAL_t val){
    int * newVal = malloc(sizeof(int));
    *newVal = val;
    (*storageEngine)->root = Put((*storageEngine)->root, key, newVal);
}

/*
 *  Put sets a key value pair. 
 *  If a key does not exist previously in the storage engine, it should be inserted.
 *  If the key exists previously, the old value should be overwritten with the newly specified value.
 * 
 *  @Params: 
 *      storageEngine      - the storage engine to operate on
 *      lowKey             - key to add
 *      highKey            - val to add
 *      rangeResult        - results, which if found, will be populated 
 *                              (see the query.h definition for more details)
 *  Returns:
 *       0 for no result, 
 *       non-zero indicating the number of the qualifying entries, held inside `rangeResult`'s keys and values
 * 
 */
// returns 0 for no result
// returns non-zero for the length of the qualifying entries
int wrapperRange(STORAGECXT_t **storageEngine, KEY_t lowKey, KEY_t highKey, RANGE_RESULT_t **rangeResult){
    // TODO GRADUATE: 
    //      You will need to define and declare a me
    //      thod(s) to run
    (void) storageEngine;
    (void) lowKey;
    (void) highKey;
    (void) rangeResult;
    return 0;
}

#endif