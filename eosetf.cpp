#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <cmath>
#include <eosio/singleton.hpp>
#include <numeric>
//#include <string> 


//REQUIRE AUTH IGALE POOLE
using namespace eosio;
using namespace std;

class [[eosio::contract("eosetf")]] eosetf : public contract {

public:
	using contract::contract;


TABLE totaleosworth{

  asset eosworth;
};
typedef eosio::singleton<"totleosworth"_n, totaleosworth> totleostab;



[[eosio::action]]
void seteosworth (asset eosworth)
{

  require_auth( _self );

  totleostab eostable(_self, _self.value);
  totaleosworth soloiter;

  if(!eostable.exists()){
    eostable.set(soloiter, _self);
  }
  else{
    soloiter = eostable.get();
  }
  soloiter.eosworth = eosworth;
  eostable.set(soloiter, _self);
}

[[eosio::action]]
void checki ()
{

totleostab eostable(_self, _self.value);
  totaleosworth soloiter;

    soloiter = eostable.get();

    check(false, soloiter.eosworth.amount);

}

[[eosio::action]]
void deleosworth ()
{
totleostab eostable(_self, _self.value);
  totaleosworth soloiter;

    soloiter = eostable.get();

//NOT SURE IF THIS WORKS
eostable.remove();

}


};
