/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
    https://github.com/ETLCPP/etl
             https://www.etlcpp.com

                      Copyright(c) 2020 jwellbelove

                      Permission is hereby granted, free of charge, to any
person obtaining a copy of this software and associated documentation files(the
"Software"), to deal in the Software without restriction, including without
limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and / or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions :

                                           The above copyright notice and this
permission notice shall be included in all copies or substantial portions of the
Software.

                                           THE SOFTWARE IS PROVIDED "AS IS",
WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
            ******************************************************************************/

#include "unit_test_framework.h"

#include "etl/fixed_sized_memory_block_allocator.h"
#include "etl/reference_counted_message_pool.h"
#include "etl/shared_message.h"

namespace {
constexpr etl::message_id_t MessageId1 = 1U;
constexpr etl::message_id_t MessageId2 = 2U;

struct Message1 : public etl::message<MessageId1> {
  Message1(int i_) : i(i_) {}

  ~Message1() {}

  int i;
};

struct Message2 : public etl::message<MessageId2> {
  ~Message2() {}
};

SUITE(test_shared_message) {
  using pool_message_parameters =
      etl::atomic_counted_message_pool::pool_message_parameters<Message1,
                                                                Message2>;

  etl::fixed_sized_memory_block_allocator<
      pool_message_parameters::max_size, pool_message_parameters::max_alignment,
      4U>
      memory_allocator;

  etl::atomic_counted_message_pool message_pool(memory_allocator);

  TEST(test_reference_counted_pool_exceptions) {
    using pool_message_parameters =
        etl::atomic_counted_message_pool::pool_message_parameters<Message1,
                                                                  Message2>;

    etl::fixed_sized_memory_block_allocator<
        pool_message_parameters::max_size,
        pool_message_parameters::max_alignment, 4U>
        memory_allocator;

    etl::atomic_counted_message_pool message_pool(memory_allocator);

    etl::reference_counted_message<Message1, etl::atomic_int>* prcm;
    CHECK_NO_THROW(prcm = message_pool.allocate<Message1>(6));

    etl::reference_counted_message<Message1, etl::atomic_int> temp(
        prcm->get_message(), message_pool);

    try {
      message_pool.release(temp);
    } catch (etl::exception e) {
      CHECK_EQUAL(std::string("reference_counted_message_pool:release failure"),
                  std::string(e.what()));
    }
  }
}
}  // namespace
