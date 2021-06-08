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

        print_result(good, title, reason);
    }

    template<typename F, typename ...Ts>
    void each(std::initializer_list<std::tuple<Ts...>> tuples, const char* pattern, F&& test)
    {
        for (const auto& tuple : tuples)
        {
            auto good = true;
            auto reason = "Uncaught exception";

            try
            {
                std::apply(test, tuple);
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

            char title[1024];

            std::apply([&title, pattern] (auto... elements) { snprintf(title, 1024, pattern, elements...); }, tuple);

            print_result(good, title, reason);
        }
    }

private:
    void print_result(bool good, const char* title, const char* reason)
    {
        if (good)
        {
            std::printf("[\e[0;32mPASSED\e[0m] %s\n", title);
        }
        else
        {
            std::printf("[\e[0;31mFAILED\e[0m] %s\n\n    %s\n\n", title, reason);
        }
    }
} it;

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


