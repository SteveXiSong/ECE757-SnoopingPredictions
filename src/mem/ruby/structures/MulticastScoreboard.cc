#include "mem/ruby/structures/MulticastScoreboard.hh"
#include "mem/ruby/system/System.hh"
#include "debug/RubySlicc.hh"

using namespace std;
typedef MulticastScoreboardParams Params;


MulticastScoreboard *
MulticastScoreboardParams::create()
{
    return new MulticastScoreboard(this);
}
