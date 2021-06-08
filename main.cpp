#include <cassert>
#include <string>
#include <stdexcept>
#include <tuple>

using namespace std::string_literals;

template <typename T>
void assert_equal__(const T& actual, const T& expected, const char* file, int line)
{
    if (actual != expected)
    {
        throw std::runtime_error("Assertion failed at "s + file + ":" + std::to_string(line) + ": " + std::to_string(actual) + " == " + std::to_string(expected));
    }
}

#define assert_equal(actual, expected) (assert_equal__(actual, expected, __FILE__, __LINE__))

struct it__
{
    void operator()(const char* title, void (*test)())
    {
        auto good = true;
        auto reason = "Uncaught exception";

        try
        {
            test();
        }
        catch (const std::exception& ex)
        {
            reason = ex.what();
            good = false;
        }
        catch (...)
        {
            good = false;
        }

        if (good)
        {
            std::printf("[\e[0;32mPASSED\e[0m] %s\n", title);

        }
        else
        {
            std::printf("[\e[0;31mFAILED\e[0m] %s\n\n    %s\n", title, reason);

        }
    }

    template<typename T>
    void each(std::initializer_list<T> elements, const char* title, void (*test)(const T&))
    {
        for (const auto& element : elements)
        {
            auto good = true;
            auto reason = "Uncaught exception";

            try
            {
                test(element);
            }
            catch (const std::exception& ex)
            {
                reason = ex.what();
                good = false;
            }
            catch (...)
            {
                good = false;
            }

            if (good)
            {
                std::printf("[\e[0;32mPASSED\e[0m] %s\n", title);

            }
            else
            {
                std::printf("[\e[0;31mFAILED\e[0m] %s\n\n    %s\n\n", title, reason);
            }
        }
    }
} it;

int main()
{
    struct calculation_data
    {
        int input_a;
        int input_b;
        int expected;
    };

    it.each<calculation_data>({
        {2, 2, 4},
        {2, 3, 5},
    }, "should calculate", [] (auto _)
    {
        assert_equal(_.input_a + _.input_b, _.expected);
    });

    it("should fail on 2 + 3", []
    {
        assert_equal(2 + 3, 4);
    });
}


