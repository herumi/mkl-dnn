/*******************************************************************************
* Copyright 2016-2018 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "mkldnn.h"

#include "c_types_map.hpp"
#include "primitive.hpp"
#include "utils.hpp"

/** \brief An abstraction of an execution unit with shared resources
 *
 * Responsibilities:
 *   - Provide engine specific memory allocation
 *   - Provide engine specific primitive_desc_t creators
 */
struct mkldnn_engine: public mkldnn::impl::c_compatible {
    mkldnn_engine(mkldnn::impl::engine_kind_t kind)
        : kind_(kind)
    {}
    virtual ~mkldnn_engine() {}

    /** get kind of the current engine */
    virtual mkldnn::impl::engine_kind_t kind() const { return kind_; }

    /* implementation section */
    virtual mkldnn::impl::status_t memory_primitive_desc_create(
            mkldnn::impl::memory_pd_t **memory_pd,
            const mkldnn::impl::memory_desc_t *memory_d)
    { return mkldnn::impl::status::unimplemented; }

    typedef mkldnn::impl::status_t (*concat_primitive_desc_create_f)(
            mkldnn::impl::concat_pd_t **concat_pd,
            const mkldnn::impl::memory_desc_t *output_d, int n, int concat_dim,
            const mkldnn::impl::memory_pd_t **input_pds,
            const mkldnn::impl::primitive_attr_t *attr);

    /** return the list of concat implementations. engine guarantees to return
     * a NULL-terminated list */
    virtual const concat_primitive_desc_create_f*
        get_concat_implementation_list() const;

    typedef mkldnn::impl::status_t (*sum_primitive_desc_create_f)(
            mkldnn::impl::sum_pd_t **sum_pd,
            const mkldnn::impl::memory_desc_t *output_d, int n,
            const float *scales, const mkldnn::impl::memory_pd_t **input_pds,
            const mkldnn::impl::primitive_attr_t *attr);

    /** return the list of sum implementations. engine guarantees to return
     * a NULL-terminated list */
    virtual const sum_primitive_desc_create_f*
        get_sum_implementation_list() const;

    typedef mkldnn::impl::status_t (*reorder_primitive_desc_create_f)(
            mkldnn::impl::reorder_pd_t **reorder_pd,
            const mkldnn::impl::memory_pd_t *input_memory_pd,
            const mkldnn::impl::memory_pd_t *output_memory_pd,
            const mkldnn::impl::primitive_attr_t *attr);

    /** return the list of reorder implementations. engine guarantees to return
     * a NULL-terminated list */
    virtual const reorder_primitive_desc_create_f*
        get_reorder_implementation_list() const;

    typedef mkldnn::impl::status_t (*primitive_desc_create_f)(
            mkldnn::impl::primitive_desc_t **, const mkldnn::impl::op_desc_t *,
            const mkldnn::impl::primitive_attr_t *attr,
            mkldnn::impl::engine_t *, const mkldnn::impl::primitive_desc_t *);

    /** return the list of implementations. engine guarantees to return a
     * NULL-terminated list */
    virtual const primitive_desc_create_f* get_implementation_list() const;

protected:
    mkldnn::impl::engine_kind_t kind_;
};

namespace mkldnn {
namespace impl {

struct engine_factory_t: public c_compatible {
    virtual size_t count() const = 0;
    virtual engine_kind_t kind() const = 0;
    virtual status_t engine_create(engine_t **engine, size_t index) const = 0;
};

}
}

#endif

// vim: et ts=4 sw=4 cindent cino^=l0,\:0,N-s
