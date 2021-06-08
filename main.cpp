#include <cassert>
#include <string>
#include <stdexcept>

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

void it(const char* title, void (*test)())
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

int main()
{
    it("should calculate 2 + 2", []
    {
        assert_equal(2 + 2, 4);
    });

    it("should fail on 2 + 3", []
    {
        assert_equal(2 + 3, 4);
    });
}


