#pragma once 

#include "tail_find.h"
#include "tail_count.h"
#include "tail_range_algo.h"
#include "tail_iterator_range.h"

using tail_find = tail_recursive_stl::find;
using tail_find_if = tail_recursive_stl::find_if;
using tail_find_if_not = tail_recursive_stl::find_if_not;
using tail_find_of = tail_recursive_stl::find_first_of;
using tail_adject_find = tail_recursive_stl::adject_find;
using tail_count = tail_recursive_stl::count;
using tail_count_if = tail_recursive_stl::count_if;
using tail_accumulate = tail_recursive_stl::accumulate;
using tail_mismatch = tail_recursive_stl::mismatch;
using tail_equal = tail_recursive_stl::equal;
using tail_advance = tail_recursive_stl::advance;
using tail_distance = tail_recursive_stl::distance;
using tail_is_permuatation = tail_recursive_stl::is_permutation;
using tail_search = tail_recursive_stl::search;
using tail_min_element = tail_recursive_stl::min_element;
using tail_adj_difference = tail_recursive_stl::adjacent_difference;
using tail_inner_product = tail_recursive_stl::inner_product;
using tail_partial_sum = tail_recursive_stl::partial_sum;
