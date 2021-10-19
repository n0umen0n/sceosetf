
[[eosio::action]]
void rebalance(name user, uint64_t pollkey, name community)

{

require_auth( user );

//CHECK IF USER IS FUND MANAGER
approvedaccs whitetbl("consortiumlv"_n, community.value);
auto whiterow = whitetbl.find(user.value);
check(whiterow != whitetbl.end(), "Account not whitelisted.");


nrofmanagtbl managtbl("consortiumlv"_n, "consortiumlv"_n.value);

const auto &itermang =managtbl.get( community.value, "No poll found with such key" );


kysimustes pollstbl("consortiumlv"_n, community.value);

const auto &iter = pollstbl.get( pollkey, "No poll found with such key" );

if (static_cast<double>(iter.nrofvoters) / itermang.nrofmanagers < 0.656)

{
check(false, "2/3 of managers have to vote in order to rebalnce.");
}

votersnulli(community,pollkey);


//VAJA VIIA NULLI NROFVOTERS p2rast rebalancingi.
//vaata mis toimub sum of all optionsiga. 


  for(int i=0; i < iter.answers.size(); i++){

//CALCULATING THE NEW ALLOCATION OF TOKENS
   double newpercentage = static_cast<double>(iter.totalvote[i]) / iter.sumofallopt;


//siin vaja et stringide asemel oleks symboolkad...
//inline action mis trigerdab consortiumlvs addition of new answer nii int kui ka sym
    auto sym = iter.answers[i];
    rebaldattaba rebaltab(get_self(), _self.value);
    auto existing = rebaltab.find( sym.code().raw() );
    
            rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.tokenpercnew    = newpercentage;
        });
        
         }
         //LOOP ENDED THAT CALCULATES NEW PERCENTAGES


totleostab eostable(_self, _self.value);
totaleosworth soloiterr;
soloiterr = eostable.get();

soloiterr.eosworth = 0;

    rebaldattaba rebaltab(get_self(), _self.value);


//CALCULATING HOW MUCH TOKENS ARE WORTH IN EOS
           for (auto iter = rebaltab.begin(); iter != rebaltab.end(); iter++)
{


pairs pairtab("swap.defi"_n, "swap.defi"_n.value);

const auto &iterpair = pairtab.get(iter->pairid, "No row with such pairid" );

//CHECK DUE TO HOW DEFIBOX TABLES ARE BUILT
//if (iterpair.reserve0.symbol == iter->token.symbol) {
if (iterpair.reserve0.symbol == iter->token) {

double eosworth = iterpair.price0_last * iter->tokeninfund;

            auto existing = rebaltab.find( iter->token.code().raw() );
rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
            s.tokenwortheos    = eosworth;
        });

}

//CHECK DUE TO HOW DEFIBOX TABLES ARE BUILT
if (iterpair.reserve1.symbol == iter->token) {

double eosworth = iterpair.price1_last * iter->tokeninfund;

            auto existing = rebaltab.find( iter->token.code().raw());
            rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
            s.tokenwortheos    = eosworth;
        });

}
//VB ERRROR
//CALCULATING TOTAL EOS WORTH OF TOKENS IN FUND
totleostab eostable(_self, _self.value);
totaleosworth soloiter;
soloiter = eostable.get();

soloiter.eosworth += iter->tokenwortheos;

}
//END OF FIRST LOOP CALCULATING TOKEN WORTH IN EOS




//LOOP CALCULATING THE OLD PERCETAGE OF TOKENS IN FUND 
//AND SELLING  / BUYING TOKENS FROM DEFIBOX
 for(int i=0; i < iter.answers.size(); i++){


 totleostab eostable(_self, _self.value);
 totaleosworth soloiter;
 soloiter = eostable.get();
 
rebaldattaba rebaltab(get_self(), _self.value);

//siia vaja see code raw
const auto &rebaliter = rebaltab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );

double tokenperold = rebaliter.tokenwortheos / soloiter.eosworth;

auto existing = rebaltab.find( iter.answers[i].code().raw() );
rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.tokenperold    = tokenperold;
 });

//SELLING TOKENS
if (rebaliter.tokenperold > rebaliter.tokenpercnew) {

double diffpertosell = rebaliter.tokenperold - rebaliter.tokenpercnew;

double perdiff = diffpertosell / rebaliter.tokenperold;

double toselldoub = rebaliter.tokeninfund * perdiff;

struct asset tosell = {int64_t (toselldoub*rebaliter.decimals), rebaliter.token};

string memo = "swap,0," + rebaliter.strpairid;


//ACTION THAT TRIGGERS SELLING
send("swap.defi"_n, _self, tosell, memo, rebaliter.contract);  


//ADJUSTING TOKENS IN FUND
auto existing = rebaltab.find( iter.answers[i].code().raw() );
rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.tokeninfund    -= toselldoub;

});
}


//BUYING TOKENS if tokenpercnew larger than 5%?
if (rebaliter.tokenperold < rebaliter.tokenpercnew) {

const auto &rebit = rebaltab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );

pairs pairtab("swap.defi"_n, "swap.defi"_n.value);

const auto &iterpair = pairtab.get(rebit.pairid, "No row with such pairid" );


double diffpertobuy = rebaliter.tokenpercnew - rebaliter.tokenperold;

double perdiff = diffpertobuy / rebaliter.tokenperold;

double eosworthtobuy = rebaliter.tokenwortheos * perdiff;

struct asset tobuy = {int64_t (eosworthtobuy * 10000), symbol ("EOS", 4)};

string memo = "swap,0," + rebaliter.strpairid;

send("swap.defi"_n, _self, tobuy, memo, "eosio.token"_n);  

//ADJUST TOKENS IN FUND BASED THE PRICE AFTER BUYING 
if (iterpair.reserve0.symbol == iter.answers[i]) {

double newprice = (static_cast<double>(tobuy.amount + iterpair.reserve1.amount * 10000) / iterpair.reserve0.amount * rebaliter.decimals);

double tokensbought = tobuy.amount / newprice;

auto existing = rebaltab.find( iter.answers[i].code().raw() );
rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.tokeninfund    += tokensbought;

});
}
//ADJUST TOKENS IN FUND BASED THE PRICE AFTER BUYING 
if (iterpair.reserve1.symbol == iter.answers[i]) {

double newprice = static_cast<double>(tobuy.amount + iterpair.reserve0.amount * 10000) / iterpair.reserve1.amount * rebaliter.decimals;

double tokensbought = tobuy.amount / newprice;

auto existing = rebaltab.find( iter.answers[i].code().raw() );
rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.tokeninfund    += tokensbought;
});
}

}
}


//LOOP TO GET MIN AMOUNTS 
    for (auto iter = rebaltab.begin(); iter != rebaltab.end(); iter++)
{

const auto &rebaliter = rebaltab.get(iter->token.code().raw(), "No pairid for such symbol" );


double mineostokworth = iter->tokenpercnew * 4;

// THOSE MAYBE NOT NEEDED
pairs pairtab("swap.defi"_n, "swap.defi"_n.value);

const auto &iterpair = pairtab.get(iter->pairid, "No row with such pairid" );


if (iterpair.reserve0.symbol == iter->token) {

double mintokenamt = mineostokworth / iterpair.price0_last;

struct asset minamount = {int64_t (rebaliter.decimals * mintokenamt), rebaliter.token};

auto existing = rebaltab.find( iter->token.code().raw() );

rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.minamount    = minamount;
});

}

if (iterpair.reserve1.symbol == iter->token) {

double mintokenamt = mineostokworth / iterpair.price1_last;

struct asset minamount = {int64_t (rebaliter.decimals * mintokenamt), rebaliter.token};

auto existing = rebaltab.find( iter->token.code().raw() );

rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.minamount    = minamount;
});

}

//LOOP TO GET MIN AMOUNTS CLOSED
}


//DELETING BASE ITER
basetoktab basetable(_self, _self.value);
basetok baseiter;

baseiter = basetable.get(); 

//NOT SURE IF THIS WORKS
basetable.remove();


/*
baseiter.base = 0;
basetable.set(baseiter, _self);
*/

//LOOP TO GET NEW RATIOS
  for(int i=0; i < iter.answers.size(); i++)
{

const auto &rebaliter = rebaltab.get(iter.answers[i].code().raw(), "No token with such symbol." );

basetoktab basetable(_self, _self.value);
basetok baseiter;

//baseiter = basetable.get(); 

if (!basetable.exists() && rebaliter.tokenpercnew > 0) 

{

basetable.set(baseiter, _self);
baseiter.base = rebaliter.minamount.symbol;
basetable.set(baseiter, _self);


}

baseiter = basetable.get(); 


const auto &itrbase = rebaltab.get(baseiter.base.code().raw(), "No token with such symbol." );

double ratio = static_cast<double>(rebaliter.minamount.amount) / itrbase.minamount.amount;

auto existing = rebaltab.find( iter.answers[i].code().raw() );

rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.ratio    = ratio;
});
//LOOP TO GET NEW RATIOS CLOSED
}


//SET SIZE *NUBMBER OF TOKENS IS THE FUND TO ZERO

etfsizetab sizetable(_self, _self.value);
etfsize soloiter;

soloiter = sizetable.get();
soloiter.size = 0;
sizetable.set(soloiter, _self);



//LOOP TO GET THE SIZE *NUBMBER OF TOKENS IS THE FUND
 for (auto iter = rebaltab.begin(); iter != rebaltab.end(); iter++)
{

if (iter->tokenpercnew > 0)

{
etfsizetab sizetable(_self, _self.value);
etfsize soloiter;

soloiter = sizetable.get();
soloiter.size += 1;
sizetable.set(soloiter, _self);
}


}
//LOOP TO GET THE SIZE *NUBMBER OF TOKENS IS THE FUND CLOSED


}


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


  struct [[eosio::table]] account {
         asset    balance;

         uint64_t primary_key()const { return balance.symbol.code().raw(); }
      };
      typedef eosio::multi_index< name("accounts"), account > accounts;

      
      
struct [[eosio::table]] currency_stats {
            asset    supply;
            asset    max_supply;
            name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };

         typedef eosio::multi_index< "stat"_n, currency_stats > stats;


struct [[eosio::table]] etfinf {
         asset    token;
         uint64_t ratio;
         uint64_t maxamount;
         uint64_t minamount;
         uint64_t multiplier;

         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };
            typedef eosio::multi_index< name("etfinfo"), etfinf > etfinfo;



struct [[eosio::table]] useritokens {
         asset    token;
         uint64_t ratio;
         uint64_t multiplier;

         

         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };

      typedef eosio::multi_index< name("userstokens"), useritokens > useritokenid;



struct [[eosio::table]] refundstab {
         asset    token;
         name contract;
         
         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };

      typedef eosio::multi_index< name("refundstab"), refundstab > refundstable;


      


TABLE pausetabla{

  bool ispaused;
};
typedef eosio::singleton<"pauze"_n, pausetabla> pausetab;


TABLE etfsize{

  int8_t size;
};
typedef eosio::singleton<"etfsize"_n, etfsize> etfsizetab;


TABLE refundrate{

  float rate;
};
typedef eosio::singleton<"refundrate"_n, refundrate> refundratetb;


TABLE totaleosworth{

  asset eosworth;
};
typedef eosio::singleton<"totleosworth"_n, totaleosworth> totleostab;


/*
TABLE basetokenint{

  int64_t base;
};
typedef eosio::singleton<"basetokint"_n, basetokenint> basetoktabint;
*/

TABLE basetok{

  symbol base;
};
typedef eosio::singleton<"basetok"_n, basetok> basetoktab;


TABLE rebalancedt {

double tokeninfund;  

double tokenwortheos;

double tokenperold;

double tokenpercnew;

int64_t decimals;

uint64_t pairid;  

string strpairid;

symbol token;

name contract;

double ratio;

asset minamount;

string image;

   
auto primary_key() const { return pairid; }

};

typedef eosio::multi_index<"rebalancedt"_n, rebalancedt> rebaldattaba;


  

//CODE FOR REBALANCING

//FINISH TABLES DEFIBOX AND TEST IF WORK. 


TABLE nrofmanagers {

uint64_t nrofmanagers;

auto primary_key() const { return nrofmanagers; }

    };

typedef eosio::multi_index<name("nrofmanagers"), nrofmanagers > nrofmanagtbl;


struct token {
      name contract;
      symbol symbol;
    };


TABLE pair {
    
  uint64_t id;

  token token0;   
  
  token token1;   

  asset reserve0;

  asset reserve1;

  uint64_t liquidity_token;

  double price0_last;

  double price1_last;

  uint64_t price0_cumulative_last;

  uint64_t price1_cumulative_last;

  time_point_sec block_time_last;
 
auto primary_key() const { return id; }
    };

  typedef eosio::multi_index<"pairs"_n, pair> pairs;



TABLE kysimuseds {
    
     uint64_t pollkey;
    
     name community;

     name creator;

     vector <uint64_t> totalvote;

     vector <symbol> answers;

     string question;

     string description;

     uint8_t nrofvoters = 0;

     uint64_t sumofallopt = 0;

     string uniqueurl;

     time_point_sec timecreated;


    auto primary_key() const { return pollkey; }

    uint64_t by_secondary( ) const { return community.value; }

    };


  typedef eosio::multi_index<"kysimuseds"_n, kysimuseds,
  eosio::indexed_by<"bycomjus"_n, eosio::const_mem_fun<kysimuseds, uint64_t, &kysimuseds::by_secondary>>> kysimustes;


TABLE white {

name accounts;

auto primary_key() const { return accounts.value; }
    };

typedef eosio::multi_index<name("approvedaccs"), white > approvedaccs;






[[eosio::action]]
void setrefundrate (float rate)
{

  require_auth( _self );

  refundratetb eostable(_self, _self.value);
  refundrate soloiter;

  if(!eostable.exists()){
    eostable.set(soloiter, _self);
  }
  else{
    soloiter = eostable.get();
  }
  soloiter.rate = rate;
  eostable.set(soloiter, _self);
}

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
void rebalance(name user, uint64_t pollkey, name community)

{

require_auth( user );

//CHECK IF USER IS FUND MANAGER
approvedaccs whitetbl("consortiumlv"_n, community.value);
auto whiterow = whitetbl.find(user.value);
check(whiterow != whitetbl.end(), "Account not whitelisted.");


nrofmanagtbl managtbl("consortiumlv"_n, "consortiumlv"_n.value);

const auto &itermang =managtbl.get( community.value, "No poll found with such key" );


kysimustes pollstbl("consortiumlv"_n, community.value);

const auto &iter = pollstbl.get( pollkey, "No poll found with such key" );

if (static_cast<double>iter.nrofvoters / itermang.nrofmanagers < 0.656)

{
check(false, "2/3 of managers have to vote in order to rebalnce.")
}

votersnulli(community,pollkey);


//VAJA VIIA NULLI NROFVOTERS p2rast rebalancingi.
//vaata mis toimub sum of all optionsiga. 


  for(int i=0; i < iter.answers.size(); i++){

//CALCULATING THE NEW ALLOCATION OF TOKENS
   double newpercentage = static_cast<double>iter.totalvote[i] / iter.sumofallopt;


//siin vaja et stringide asemel oleks symboolkad...
//inline action mis trigerdab consortiumlvs addition of new answer nii int kui ka sym
    auto sym = iter.answers[i].symbol;
    rebaldattaba rebaltab(get_self(), _self.value);
    auto existing = rebaltab.find( sym.code().raw() );
    
            rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.tokenpercnew    = newpercentage;
        });
        
         }
         //LOOP ENDED THAT CALCULATES NEW PERCENTAGES


totleostab eostable(_self, _self.value);
totaleosworth soloiter;
soloiter = eostable.get();

soloiter.eosworth = 0;



//CALCULATING HOW MUCH TOKENS ARE WORTH IN EOS
           for (auto iter = rebaltab.begin(); iter != rebaltab.end(); iter++)
{

const auto &iterpair = pairtab.get(iter->pairid, "No row with such pairid" );

//CHECK DUE TO HOW DEFIBOX TABLES ARE BUILT
//if (iterpair.reserve0.symbol == iter->token.symbol) {
if (iterpair.reserve0.symbol == iter->token) {




double eosworth = iterpair.price0_last * iter->tokensinfund;

            auto existing = rebaltab.find( iter->token.code().raw() );
rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
            s.tokenwortheos    = eosworth;
        });

}

//CHECK DUE TO HOW DEFIBOX TABLES ARE BUILT
if (iterpair.reserve1.symbol == iter->token) {

double eosworth = iterpair.price1_last * iter->tokensinfund;

            auto existing = rebaltab.find( iter->token.code().raw());
            rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
            s.tokenwortheos    = eosworth;
        });

}
//CALCULATING TOTAL EOS WORTH OF TOKENS IN FUND
totleostab eostable(_self, _self.value);
totaleosworth soloiter;
soloiter = eostable.get();

soloiter.eosworth += iter->tokenwortheos;

}
//END OF FIRST LOOP CALCULATING TOKEN WORTH IN EOS




//LOOP CALCULATING THE OLD PERCETAGE OF TOKENS IN FUND 
//AND SELLING  / BUYING TOKENS FROM DEFIBOX
 for(int i=0; i < iter.answers.size(); i++){


 totleostab eostable(_self, _self.value);
 totaleosworth soloiter;
 soloiter = eostable.get();

const auto &rebaliter = rebaltab.get(iter.answers[i].symbol, "No pairid for such symbol" );

double tokenperold = rebaliter.tokenwortheos / soloiter.eosworth;

auto existing = rebaltab.find( iter.answers[i].symbol.code().raw() );
rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.tokenperold    = tokenperold;
 }

//SELLING TOKENS
if (rebaliter.tokenperold > rebaliter.tokenpernew) {

double diffpertosell = rebaliter.tokenperold - rebaliter.tokenpernew;

double perdiff = diffpertosell / rebaliter.tokenperold;

double toselldoub = rebaliter.tokeninfund * perdiff;

struct asset tosell = {int64_t (toselldoub*rebaliter.decimals), rebaliter.toksymbol};

string memo = "swap,0," + rebaliter.strpairid;


//ACTION THAT TRIGGERS SELLING
send("swap.defi"_n, _self, tosell, memo, rebaliter.contract);  


//ADJUSTING TOKENS IN FUND
auto existing = rebaltab.find( iter.answers[i].symbol.code().raw() );
rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.tokeninfund    -= toselldoub;

 }


//BUYING TOKENS if tokenpernew larger than 5%?
if (rebaliter.tokenperold < rebaliter.tokenpernew) {

double diffpertobuy = rebaliter.tokenpernew - rebaliter.tokenperold;

double perdiff = diffpertobuy / rebaliter.tokenperold;

double eosworthtobuy = rebaliter.tokenwortheos * perdiff;

struct asset tobuy = {int64_t (eosworthtobuy * 10000), symbol ("EOS", 4)};

string memo = "swap,0," + rebaliter.strpairid;

send("swap.defi"_n, _self, tobuy, memo, "eosio.token"_n);  

//ADJUST TOKENS IN FUND BASED THE PRICE AFTER BUYING 
if (iterpair.reserve0.symbol == iter.answers[i].symbol) {

double newprice = static_cast<double>(tobuy.amount + iterpair.reserve1.amount * 10000) / iterpair.reserve0.amount * rebaliter.decimals;

double tokensbought = tobuy.amount / newprice;

auto existing = rebaltab.find( iter.answers[i].symbol.code().raw() );
rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.tokeninfund    += tokensbought;

}
//ADJUST TOKENS IN FUND BASED THE PRICE AFTER BUYING 
if (iterpair.reserve1.symbol == iter.answers[i].symbol) {

double newprice = static_cast<double>(tobuy.amount + iterpair.reserve0.amount * 10000) / iterpair.reserve1.amount * rebaliter.decimals;

double tokensbought = tobuy.amount / newprice;

auto existing = rebaltab.find( iter.answers[i].symbol.code().raw() );
rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.tokeninfund    += tokensbought;

}


}


//LOOP TO GET MIN AMOUNTS 
    for (auto iter = rebaltab.begin(); iter != rebaltab.end(); iter++)
{

//const auto &rebaliter = rebaltab.get(iter->answers[i].symbol, "No pairid for such symbol" );

double mineostokworth = iter->tokenpernew * 4;

// THOSE MAYBE NOT NEEDED
pairs pairtab("swap.defi"_n, "swap.defi"_n.value);

const auto &iterpair = pairtab.get(iter->pairid, "No row with such pairid" );


if (iterpair.reserve0.symbol == iter->token) {

double mintokenamt = mineostokworth / iterpair.price0_last;

struct asset minamount = {int64_t (rebaliter.decimals * mintokenamt), rebaliter.token};

auto existing = rebaltab.find( iter->token.code().raw() );

rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.minamount    = minamount;

}

}

if (iterpair.reserve1.symbol == iter->token) {

double mintokenamt = mineostokworth / iterpair.price1_last;

struct asset minamount = {int64_t (rebaliter.decimals * mintokenamt), rebaliter.token};

auto existing = rebaltab.find( iter->token.code().raw() );

rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.minamount    = minamount;

}

}

//LOOP TO GET MIN AMOUNTS CLOSED
}


//DELETING BASE ITER
basetoktab basetable(_self, _self.value);
basetok baseiter;

baseiter = basetable.get(); 

//NOT SURE IF THIS WORKS
basetable.remove();


/*
baseiter.base = 0;
basetable.set(baseiter, _self);
*/

//LOOP TO GET NEW RATIOS
  for(int i=0; i < iter.answers.size(); i++){
{

const auto &rebaliter = rebaltab.get(iter.answers[i].symbol, "No token with such symbol." );

basetoktab basetable(_self, _self.value);
basetok baseiter;

//baseiter = basetable.get(); 

if (!basetable.exists() && rebaliter.tokenpernew > 0) 

{

basetable.set(baseiter, _self);
baseiter.base = rebaliter.minamount.amount;
basetable.set(baseiter, _self);


}

baseitrnew = basetable.get(); 

double ratio = static_cast<double>(rebaliter.minamount.amount) / baseitrnew.minamount;

auto existing = rebaltab.find( iter.answers[i].symbol.code().raw() );

rebaltab.modify(existing,name("cet.f"), [&]( auto& s ){
              s.ratio    = ratio;

}
//LOOP TO GET NEW RATIOS CLOSED



//SET SIZE *NUBMBER OF TOKENS IS THE FUND TO ZERO

etfsizetab sizetable(_self, _self.value);
etfsize soloiter;

soloiter = sizetable.get();
soloiter.size = 0;
sizetable.set(soloiter, _self);



//LOOP TO GET THE SIZE *NUBMBER OF TOKENS IS THE FUND
 for (auto iter = rebaltab.begin(); iter != rebaltab.end(); iter++)
{

if (iter->tokenpernew > 0)

{
etfsizetab sizetable(_self, _self.value);
etfsize soloiter;

soloiter = sizetable.get();
soloiter.size += 1;
sizetable.set(soloiter, _self);
}


}
//LOOP TO GET THE SIZE *NUBMBER OF TOKENS IS THE FUND CLOSED




[[eosio::action]]
void addtokens(vector <double> tokeninfund, vector <double> tokenwortheos, vector <double> tokenperold, vector <double> tokenpercnew, vector <int64_t> decimals, vector <uint64_t> pairid,
vector <string>  strpairid, vector <symbol> token, vector <name> contract, vector <double> ratio, vector <asset> minamount)
{

  require_auth ( _self);

  for (size_t i = 0; i < pairid.size(); ++i)

  {

     rebaldattaba rebaltab(get_self(), _self.value);
      auto existing = rebaltab.find( token[i].code().raw() );
         

     if(existing==rebaltab.end() ) {
         rebaltab.emplace( _self, [&]( auto& s ) {
             s.tokeninfund    = tokeninfund[i];
             s.tokenwortheos    = tokenwortheos[i];
             s.tokenperold    = tokenperold[i];
             s.tokenpercnew    = tokenpercnew[i];
             s.decimals    = decimals[i];
             s.pairid    = pairid[i];
             s.strpairid    = strpairid[i];
             s.token    = token[i];
             s.contract    = contract[i];
             s.ratio    = ratio[i];
             s.minamount    = minamount[i];
      
        });

                                   }


 }


sendsymtotvot(sym,totalvote);

}



[[eosio::action]]
void pause(bool ispaused)
{

  require_auth( _self );

pausetab pausetable(_self, _self.value);
  pausetabla soloiter;
  if(!pausetable.exists()){
    pausetable.set(soloiter, _self);
  }
  else{
    soloiter = pausetable.get();
  }
  soloiter.ispaused = ispaused;
  pausetable.set(soloiter, _self);
}


[[eosio::action]]
void create( const name&   issuer,
                    const asset&  maximum_supply )
{
    require_auth( _self );

    auto sym = maximum_supply.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( maximum_supply.is_valid(), "invalid supply");
    check( maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( _self, [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = issuer;
    });
}

[[eosio::action]]
void issue( const name& to, const asset& quantity, const string& memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;
    check( to == st.issuer, "tokens can only be issued to issuer account" );

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must issue positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );
}



[[eosio::action]]
void transfer( name    from,
                      name    to,
                      asset   quantity,
                      std::string  memo )
{
   check( from != to, "cannot transfer to self" );
   require_auth( from );
   check( is_account( to ), "to account does not exist");
   auto sym = quantity.symbol.code();
   stats statstable( _self, sym.raw() );
   const auto& st = statstable.get( sym.raw() );

   require_recipient( from );
   require_recipient( to );

   check( quantity.is_valid(), "invalid quantity" );
   check( quantity.amount > 0, "must transfer positive quantity" );
   check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
   check( memo.size() <= 256, "memo has more than 256 bytes" );

   auto payer = has_auth( to ) ? to : from;

   auto separator_pos = memo.find(':');


if ( to == get_self()){

//check(false, "This action will be activated when CETF distribution ends or latest on 31.04.2021");
        
        refund_tokens_back (from, to, quantity, memo);
        
        

     sub_balance( from, quantity );
   add_balance( to, quantity, payer );

   retire (quantity,memo);

  }
  else{
     sub_balance( from, quantity );
   add_balance( to, quantity, payer );
  }
   // default transfer
   
}




//UUS
[[eosio::on_notify("dappservices::transfer")]]
void issueetfdapp (name from, name to, asset quantity, const string memo)

{   

if (from != "thedappfund1"_n)

{

savetokens(from, quantity,to);

}

}

[[eosio::on_notify("effecttokens::transfer")]]
void issueetfefx (name from, name to, asset quantity, std::string memo){
   

savetokens(from, quantity,to);

}


[[eosio::on_notify("core.ogx::transfer")]]
void issueetfogx (name from, name to, asset quantity, std::string memo){

savetokens(from, quantity,to);

}


[[eosio::on_notify("everipediaiq::transfer")]]
void issueetfiq (name from, name to, asset quantity, std::string memo){

savetokens(from, quantity,to);

}


[[eosio::on_notify("vig111111111::transfer")]]
void issueetfvig (name from, name to, asset quantity, std::string memo){

savetokens(from, quantity,to);

}

[[eosio::on_notify("token.defi::transfer")]]
void issueetfbox (name from, name to, asset quantity, std::string memo){
     
     savetokens(from, quantity,to);

}

[[eosio::on_notify("dadtoken1111::transfer")]]
void issueetfdad (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}

[[eosio::on_notify("pizzatotoken::transfer")]]
void issueetfpizza (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}

[[eosio::on_notify("chexchexchex::transfer")]]
void issueetfchex (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}


[[eosio::on_notify("token.newdex::transfer")]]
void issueetfdex (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}

[[eosio::on_notify("eosiotptoken::transfer")]]
void issueetftpt (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}

[[eosio::on_notify("emanateoneos::transfer")]]
void issueetfemt (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}

[[eosio::on_notify("minedfstoken::transfer")]]
void issueetfdfs (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}



private:

    void refund_tokens_back(name from, name to, asset quantity, std::string memo) {

    check (quantity.symbol == symbol("EOSETF", 4), "Token with wrong symbol.");

    check (quantity.amount >= 10000, "Can't redeem less than 1 EOSETF" );

    check (quantity.amount <= 2000000, "Can't more than 200 EOSETF" );

    pauseornot();


    rebaldattaba reftab(get_self(), _self.value);


  refundratetb eostable(_self, _self.value);
  refundrate soloiter;

  
    soloiter = eostable.get();
 

    for (auto iter = reftab.begin(); iter != reftab.end(); iter++)
{

if (iter->tokenpernew > 0)

{
     struct asset refundasset = {int64_t ((quantity.amount * iter->minamount.amount*soloiter.rate)/10000), iter->token);

     send(to, from, refundasset, memo, iter->contract);  

}

}

}



void sub_balance( name owner, asset value ) 
{
   accounts from_acnts( _self, owner.value );
   const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found in accounts" );

   

      check( from.balance >= ( value ), "sub_balance: from.balance overdrawn balance" );
  

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
   });
}


void add_balance( name owner, asset value, name ram_payer )
{
   accounts to_acnts( _self, owner.value );
   auto to = to_acnts.find( value.symbol.code().raw() );
   if( to == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });

   } else {
      to_acnts.modify( to, same_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
}






void retire( asset quantity, std::string memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist" );
    const auto& st = *existing;

    //require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must retire positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply -= quantity;
    });

    sub_balance( st.issuer, quantity );
}



void createetf( name owner, asset quantity )
    {
        auto sym = quantity.symbol;
        check( sym.is_valid(), "Invalid symbol name" );

        auto sym_code_raw = sym.code().raw();

        stats statstable( _self, sym_code_raw );
        auto existing = statstable.find( sym_code_raw );
        check( existing != statstable.end(), "Token with that symbol name does not exist - Please create the token before issuing" );

        const auto& st = *existing;
        
        check( quantity.is_valid(), "Invalid quantity value" );
        check( st.supply.symbol == quantity.symbol, "Symbol precision mismatch" );
        check( st.max_supply.amount - st.supply.amount >= quantity.amount, "Quantity value cannot exceed the available supply" );

        statstable.modify( st, name("cet.f"), [&]( auto& s ) {
            s.supply += quantity;
        });
        
        
      
       add_balance( owner, quantity, name("cet.f"));

       //add_balance( owner, quantity, owner);
    }


void pauseornot( ) {

pausetab pauztab(_self, _self.value);
pausetabla iter;

iter = pauztab.get();

check(iter.ispaused, "Creation and redemption is currently halted.");
}





void savetokens( name from, asset quantity, name to )
    {
if (to  != "cet.f"_n) return;

pauseornot();

  rebaldattaba sinput(get_self(), _self.value);
      auto secinput = sinput.find( quantity.symbol.code().raw() );

    useritokenid input(get_self(), from.value);
      auto newinput = input.find( quantity.symbol.code().raw() );

 check (quantity.symbol == secinput->token.symbol, "Incorrect symbol.");

 //check (quantity.amount <= secinput->maxamount, "Maximum creation threshold is 200 EOSETF.");

 check (quantity.amount >= secinput->minamount.amount, "Minimum creation threshold is 1 EOSETF.");

if (secinput->tokenpernew > 0) 

{

     if( newinput == input.end() ) {
         input.emplace( name("cet.f"), [&]( auto& a ){
            a.token= quantity;
            a.ratio = secinput->ratio;
         });

     }
         else{
            input.modify(newinput,name("cet.f"), [&]( auto& a ){
            a.token += quantity;
            a.ratio = secinput->ratio;
        });
      }

}

checkratuus (from);
}


void checkratuus( name from )
    {

basetoktab basetable(_self, _self.value);
basetok baseiter;

baseiter = basetable.get();    

    
useritokenid input(get_self(), from.value);

auto size = std::distance(input.cbegin(),input.cend());

etfsizetab sizetable(_self, _self.value);
etfsize soloiter;

soloiter = sizetable.get();

//if size is smaller will not issue CETF/EOSETF, just saves the token values in the table. 
if (size == soloiter.size ) {

const auto& basetokrow = input.find(baseiter.base.code().raw() );

for (auto iter = input.begin(); iter != input.end();)
{

check (iter->token.amount != 0, "Doggy Afuera!");

check ((static_cast<double>iter->token.amount / basetokrow->token.amount == iter->ratio), "Incorrect token ratios.");


input.erase(iter++);

}

//CHECK THIS AGAIN
rebaldattaba rebaltab(get_self(), _self.value);
    auto existing = rebaltab.find( baseiter.base.code().raw());



//CHANGE IF BASETOKROW CHANGES FROM DFS
struct asset numberofetfs = {int64_t ((basetokrow->token.amount/existing.minamount.amount)*10000), symbol ("EOSETF", 4)};

createetf(from, numberofetfs );

auto sym = symbol ("CETF", 4);


stats statstable( _self, sym.code().raw() );
auto existing = statstable.find( sym.code().raw() );
const auto& st = *existing;

if (st.supply.amount < 800000000000)

{

const int64_t interval = (200000000000);  

int64_t halvings =  (st.supply.amount / interval);

int64_t rewardint =  (800000);

int64_t divider = pow( 2 , halvings);

int64_t adjrewardint = rewardint/divider;

struct asset reward = {int64_t (adjrewardint*numberofetfs.amount/10000), symbol ("CETF", 4)};

createetf(from, reward );

}
}
}

 void send(name from, name to, asset quantity, std::string memo, name contract) {
    
      action(
      permission_level{get_self(),"active"_n},
      contract,
      "transfer"_n,
      std::make_tuple(from,to,quantity,memo)
    ).send();
  };


 void sendsymtotvot(vector <symbol> sym, vector <uint64_t> totalvote) {
    
      action(
      permission_level{get_self(),"active"_n},
      "consortiumlv"_n,
      "addtokenvec"_n,
      std::make_tuple(sym,totalvote)
    ).send();
  };


void votersnulli(name community, uint64_t pollkey) {
    
      action(
      permission_level{get_self(),"active"_n},
      "consortiumlv"_n,
      "votersnulli"_n,
      std::make_tuple(community ,pollkey)
    ).send();
  };








};
