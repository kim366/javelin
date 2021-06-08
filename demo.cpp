#include "javelin.hpp"

int main()
{
    it.each({
        std::make_tuple(2, 2, 4),
        std::make_tuple(2, 3, 5),
    }, "should calculate %d + %d = %d", [] (auto input_a, auto input_b, auto expected)
    {
        assert_equal(input_a + input_b, expected);
    });

    it("should fail on 2 + 3", []
    {
        assert_equal(2 + 3, 4);
    });
}
