#include "smart_ptr.h"
#include "shape.h"
#include <utility>

int main (int argc, char *argv[]) {
    smart_ptr<shape> ptr1{create_shape(shape_type::circle)};
    smart_ptr<shape> ptr2{ptr1};

    smart_ptr<shape> ptr3{create_shape(shape_type::circle)};
    smart_ptr<circle> ptr4;
    ptr3 = ptr4;

    smart_ptr<shape> ptr5{create_shape(shape_type::circle)};
    smart_ptr<circle> ptr6;
    smart_ptr<triangle> ptr7;
    // ptr7 = ptr6;

    // std::move(Tp &&t)
    return 0;
}
