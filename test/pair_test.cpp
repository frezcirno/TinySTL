#include "pair.hpp"
#include <tuple>


int main(int argc, char const *argv[])
{
    tinySTL::pair<int, char> p_i_c;
    tinySTL::pair<const int, char> p_ci_c;
    // std::pair<int, char> p_i_c;
    // std::pair<const int, char> p_ci_c;

    p_i_c = p_ci_c;
    p_ci_c = p_i_c;

    return 0;
}
