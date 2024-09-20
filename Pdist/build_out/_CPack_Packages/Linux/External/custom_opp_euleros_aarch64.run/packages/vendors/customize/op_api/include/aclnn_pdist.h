
/*
 * calution: this file was generated automaticlly donot change it.
*/

#ifndef ACLNN_PDIST_H_
#define ACLNN_PDIST_H_

#include "aclnn/acl_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* funtion: aclnnPdistGetWorkspaceSize
 * parameters :
 * x : required
 * p : optional
 * y : required
 * workspaceSize : size of workspace(output).
 * executor : executor context(output).
 */
__attribute__((visibility("default")))
aclnnStatus aclnnPdistGetWorkspaceSize(
    const aclTensor *x,
    double p,
    const aclTensor *y,
    uint64_t *workspaceSize,
    aclOpExecutor **executor);

/* funtion: aclnnPdist
 * parameters :
 * workspace : workspace memory addr(input).
 * workspaceSize : size of workspace(input).
 * executor : executor context(input).
 * stream : acl stream.
 */
__attribute__((visibility("default")))
aclnnStatus aclnnPdist(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    const aclrtStream stream);

#ifdef __cplusplus
}
#endif

#endif
