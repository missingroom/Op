
/*
 * calution: this file was generated automaticlly donot change it.
*/

#ifndef ACLNN_DEPTH_TO_SPACE_H_
#define ACLNN_DEPTH_TO_SPACE_H_

#include "aclnn/acl_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* funtion: aclnnDepthToSpaceGetWorkspaceSize
 * parameters :
 * x : required
 * blockSize : required
 * mode : optional
 * dataFormat : optional
 * y : required
 * workspaceSize : size of workspace(output).
 * executor : executor context(output).
 */
__attribute__((visibility("default")))
aclnnStatus aclnnDepthToSpaceGetWorkspaceSize(
    const aclTensor *x,
    int64_t blockSize,
    char *mode,
    char *dataFormat,
    const aclTensor *y,
    uint64_t *workspaceSize,
    aclOpExecutor **executor);

/* funtion: aclnnDepthToSpace
 * parameters :
 * workspace : workspace memory addr(input).
 * workspaceSize : size of workspace(input).
 * executor : executor context(input).
 * stream : acl stream.
 */
__attribute__((visibility("default")))
aclnnStatus aclnnDepthToSpace(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    const aclrtStream stream);

#ifdef __cplusplus
}
#endif

#endif
