#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <cmath>
#include <eosio/singleton.hpp>
#include <numeric>

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



struct [[eosio::table]] usertokens {
         asset    token;
         double ratio;
        
         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };

      typedef eosio::multi_index< name("usertokens"), usertokens > useritokans;

//STAKING TABLES
TABLE prstotstkd {

    name staker; 

    asset indtotstaked;

    auto primary_key() const { return staker.value; }

    };

    typedef eosio::multi_index<name("prstotstkd"), prstotstkd > perstotlskd;


TABLE perzonstaked {

    uint64_t id;

    asset staked;

    time_point_sec staketime;

    uint64_t stakeperiod;  


    auto primary_key() const { return id; }

    };

    typedef eosio::multi_index<name("persznstaked"), perzonstaked > perzonstkd;


TABLE claimtime {

    name user;

    uint64_t claimperiod;  


    auto primary_key() const { return user.value; }

    };

    typedef eosio::multi_index<name("claimtime"), claimtime > claimtimetb;



TABLE totalstk {

  asset totalstaked = {int64_t (00000), symbol ("CETF", 4)};
};
typedef eosio::singleton<"totalstk"_n, totalstk> totalstk_def;



TABLE divperiod {

  time_point_sec periodstart;

  uint64_t claimperiod;  

};

typedef eosio::singleton<"divperiod"_n, divperiod> divperiod_def;

TABLE feesadjust {

  asset adjustcrtclm = {int64_t (00000), symbol ("EOSETF", 4)};
};
typedef eosio::singleton<"feesadjust"_n, feesadjust> feesadjust_def;



TABLE etffees {

  asset totalfees = {int64_t (00000), symbol ("EOSETF", 4)};
};
typedef eosio::singleton<"etffees"_n, etffees> etffees_def;


//STAKING TABLES END





//I think too many singletons.
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

  double eosworth;
};
typedef eosio::singleton<"totleosworth"_n, totaleosworth> totleostab;


TABLE clmperfreq {

int64_t periodfreq;
};
typedef eosio::singleton<"clmperfreq"_n, clmperfreq> divperiodfrq_def;


TABLE basetok{

  symbol base;
};
typedef eosio::singleton<"basetok"_n, basetok> basetoktab;

TABLE etfprice{

  double one;
};
typedef eosio::singleton<"etfprice"_n, etfprice> etfpricetb;


//In this table tokeninfund should be UINT64 and token symbol is actually present in asset as such not needed separately to stare in SYMBOL TOKEN.
TABLE rebalon {

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

   
auto primary_key() const { return token.code().raw(); }

};

typedef eosio::multi_index<"rebalon"_n, rebalon> rebalontb;



TABLE mngtab {

uint64_t nrofmanagers;

name community;

auto primary_key() const { return community.value; }

    };

typedef eosio::multi_index<name("mngtab"), mngtab > nrofmngtab;




//TABLES OF OTHER CONTRACTS
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





//SETS HOW MUCH 1 EOSETF SHOULD COST
[[eosio::action]]
void setetfprice (double one)
{

  require_auth( _self );

  etfpricetb eostable(_self, _self.value);
  etfprice soloiter;

  if(!eostable.exists()){
    eostable.set(soloiter, _self);
  }
  else{
    soloiter = eostable.get();
  }
  soloiter.one = one;
  eostable.set(soloiter, _self);
}


//SET WHEN DIVIDEND DISTRIBUTION PERIOD STARTS AND WHICH PERIOD IS IT
[[eosio::action]]
void setdivper (uint64_t claimperiod)
{
require_auth( _self );
//
divperiod_def divpertb(_self, _self.value);
divperiod divperiter;

  if(!divpertb.exists()){
divpertb.set(divperiter, _self);
  }
  else{
    divperiter = divpertb.get();
  }
  divperiter.periodstart = current_time_point();  
  divperiter.claimperiod = claimperiod;

divpertb.set(divperiter, _self);
}


//SET HOW FREQUENTLY CAN DIVIDENDS BE CLAIMED
[[eosio::action]]
void setdivperfrq (int64_t claimfreq)
{
require_auth( _self );
//
divperiodfrq_def divperfqtb(_self, _self.value);
clmperfreq divperfrqit;

  if(!divperfqtb.exists()){
divperfqtb.set(divperfrqit, _self);
  }
  else{
    divperfrqit = divperfqtb.get();
  }
  divperfrqit.periodfreq = claimfreq;  

divperfqtb.set(divperfrqit, _self);
}





//SET HOW MUCH CETF IS STAKED IN TOTAL IN ORDER TO CLAIM DIVIDENDS
[[eosio::action]]
void settotstkd (asset quantity)
{

require_auth( _self );
//
totalstk_def totalstktbl(_self, _self.value);
totalstk newstats;

  if(!totalstktbl.exists()){
totalstktbl.set(newstats, _self);
  }
  else{
    newstats = totalstktbl.get();
  }
  newstats.totalstaked = quantity;
totalstktbl.set(newstats, _self);
}


//SET HOW MUCH FEES HAVE BEEN ACCUMULATED (EOSETF)
[[eosio::action]]
void settotfeeamt (asset quantity)
{
require_auth( _self );
//
etffees_def totfeestb(_self, _self.value);
etffees totfeeiter;

  if(!totfeestb.exists()){
totfeestb.set(totfeeiter, _self);
  }
  else{
    totfeeiter = totfeestb.get();
  }
  totfeeiter.totalfees = quantity;
totfeestb.set(totfeeiter, _self);
}



//SET BY HOW MUCH SHOULD THE TOTAL FEES AMOUNT BE ADJUSTED (EOSETF) DURING THE CLAIM PERIOD adjustcrtclm 
//increases if somebody creates EOSETF and decreases if somebody claims the amount. at the end of the claiming period totalfees - adjustcrtclm
[[eosio::action]]
void seteosetfadj (asset quantity)
{

require_auth( _self );

feesadjust_def etffeestb(_self, _self.value);
feesadjust feeitr;

  if(!etffeestb.exists()){
etffeestb.set(feeitr, _self);
  }
  else{
    feeitr = etffeestb.get();
  }
  feeitr.adjustcrtclm = quantity;
etffeestb.set(feeitr, _self);
}

//SET FEE RATE FOR REDEMPTION
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

//SET WHAT IS THE TOTAL WORTH OF TOKENS IN THE FUND
[[eosio::action]]
void seteosworth (double eosworth)
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





//STAKE CETF IN ORDER TO RECEIVE FEES ACCUMULATED FOR CREATING EOSETF
//EACH STAKE HAS SEPARATE ROW
//
[[eosio::action]]
void stakecetf(name user, asset quantity, uint64_t id){


require_auth ( user );

    auto sym = quantity.symbol.code();
    auto symcetf = symbol ("CETF", 4);


    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must stake positive quantity" );
    check( quantity.symbol == symcetf, "Only possible to stake CETF." );


accounts from_acnts( _self, user.value );
const auto& from = from_acnts.get( quantity.symbol.code().raw(), "no balance object found" );


check( from.balance.amount >= quantity.amount, "Staking more CETF than you have." );


divperiod_def divpertb(_self, _self.value);
divperiod divperiter;
divperiter = divpertb.get();


perzonstkd personstktbl(_self, user.value);
auto userrow = personstktbl.find(id);
if(userrow==personstktbl.end() ) {
personstktbl.emplace( _self, [&]( auto& s ) {
             s.id    = id;                            
             s.staked    = quantity;                            
             s.staketime    = current_time_point();          
             s.stakeperiod = divperiter.claimperiod;                  
         });

}
     if(userrow!=personstktbl.end() ) {
check(false, "This ID already in use, please try staking again.");

     }


for (auto iter = personstktbl.begin(); iter != personstktbl.end(); iter++)
{


perstotlskd perstottb(_self, _self.value);
auto totrow = perstottb.find(user.value);

if(totrow==perstottb.end() ) {
perstottb.emplace( _self, [&]( auto& s ) {
             s.indtotstaked    = iter->staked;
             s.staker    = user;
 
         });
}
     if(totrow!=perstottb.end() ) {
     perstottb.modify(totrow,name("consortiumtt"), [&]( auto& s ){
             s.indtotstaked += iter->staked;
         });
}

}


perstotlskd indtotstk(_self, _self.value);
const auto &pede =indtotstk.get(user.value, "Individual has not staked." );


check( from.balance.amount >= pede.indtotstaked.amount, "Trying to stake more than available CETF." );

totalstk_def totalstktbl(_self, _self.value);
totalstk newstats;

newstats = totalstktbl.get();
newstats.totalstaked.amount += quantity.amount;
totalstktbl.set(newstats, _self);


auto totalrow = indtotstk.find(user.value);
indtotstk.modify(totalrow,name("consortiumtt"), [&]( auto& s ){
  s.indtotstaked.amount = 0;
         });
}








[[eosio::action]]
void unstakecetf(name user, vector <asset> quantity, vector <uint64_t> id){


require_auth ( user );

    
  for(int i=0; i < quantity.size(); i++){


    auto sym = quantity[i].symbol.code();
    stats statstable( _self, sym.raw() );
    const auto& st = statstable.get( sym.raw() );

    check( quantity[i].is_valid(), "invalid quantity" );
    check( quantity[i].amount > 0, "must ustake positive quantity" );
    check( quantity[i].symbol == st.supply.symbol, "symbol precision mismatch while staking" );

    accounts from_acnts( _self, user.value );
   const auto& from = from_acnts.get( quantity[i].symbol.code().raw(), "no balance object found" );



perzonstkd personstktbl(_self, user.value);

auto userrow = personstktbl.find(id[i]);

const auto &iterone =personstktbl.get(id[i], "No such staking ID(1)." );

check(iterone.staked.amount >= quantity[i].amount, "Unstaking too much CETF.");

  personstktbl.modify(userrow,name("consortiumtt"), [&]( auto& s ){
             s.staked.amount -= quantity[i].amount;
         });


const auto &itertwo =personstktbl.get(id[i], "No such staking ID(2)." );

if (itertwo.staked.amount == 0) {

   personstktbl.erase(userrow);

}

totalstk_def totalstktbl(_self, _self.value);
totalstk newstats;

newstats = totalstktbl.get();

  newstats.totalstaked.amount -= quantity[i].amount;
  totalstktbl.set(newstats, _self);


}


}



//FUNCTION THAT ENABLES TO CLAIM FEES IF USER STAKED CETF. 
//FEES CAN BE CLAIMED ANYTIME.
//FEES THAT ARE CLAIMED ARE FOR THE PREVIOUS PERIOD.
//IF USER DOES NOT CLAIM THE FEES HE PARTIALLY LOSES OUT, EVEN THOUGH HIS PART THAT WAS NOT CLAIMED IS CARRIED OVER TO THE NEXT PERIOD.
[[eosio::action]]
void getdiv(name user)

{

require_auth ( user );


//GET WHEN CURRENT PERIOD STARTED
divperiod_def divpertb(_self, _self.value);
divperiod divperiter;
divperiter = divpertb.get();

divperiodfrq_def divperfqtb(_self, _self.value);
clmperfreq divperfrqit;
divperfrqit = divperfqtb.get();

claimtimetb claimtab(_self, _self.value);
auto claimrow = claimtab.find(user.value);

//CHECK IF PERIOD IS STILL ON OR NEW HAS TO START
if (divperiter.periodstart + divperfrqit.periodfreq > current_time_point()) {



if(claimrow==claimtab.end() ) {
claimtab.emplace( _self, [&]( auto& s ) {
             s.claimperiod    = divperiter.claimperiod; 
             s.user = user;                           
         });
}
//NEW PERIOD NOT STARTED AND USER TRIES TO CLAIM AGAIN.
     if(claimrow!=claimtab.end() ) {

const auto& claimiter = claimtab.get( user.value, "User has not staked nah" );

check(claimiter.claimperiod != divperiter.claimperiod, "New period not started yet.");

}


}







//TRIGGERING START OF A NEW PERIOD
else {


  divperiter.periodstart = current_time_point();
  divperiter.claimperiod += 1;
  divpertb.set(divperiter, _self);

if(claimrow==claimtab.end() ) {
claimtab.emplace( _self, [&]( auto& s ) {
             s.claimperiod    = divperiter.claimperiod; 
             s.user = user;                           
         });
}
     if(claimrow!=claimtab.end() ) {

 claimtab.modify(claimrow,name("consortiumtt"), [&]( auto& s ){
             s.claimperiod    = divperiter.claimperiod;             
         });
}

}


perzonstkd personstktbl(_self, user.value);


//CALCULATE HOW MUCH IN TOTAL USER HAS STAKED, SO HE COULD NOT STAKE MORE.
 for (auto iter = personstktbl.begin(); iter != personstktbl.end(); iter++)
{


if (iter->staketime + divperfrqit.periodfreq  < current_time_point() && divperiter.claimperiod != iter->stakeperiod) {

perstotlskd perstottb(_self, _self.value);
auto totrow = perstottb.find(user.value);

if(totrow==perstottb.end() ) {
perstottb.emplace( _self, [&]( auto& s ) {
             s.indtotstaked    = iter->staked;                            
         });
}
     if(totrow!=perstottb.end() ) {
     perstottb.modify(totrow,name("consortiumtt"), [&]( auto& s ){
             s.indtotstaked += iter->staked;
         });
}

}

}



totalstk_def totalstktbl(_self, _self.value);
totalstk newstats;

newstats = totalstktbl.get();


//Multiple times declared, should be put on top?
perstotlskd indtotstk(_self, _self.value);
const auto &iter =indtotstk.get(user.value, "Individual has not staked, or stake has not matured." );


double percgets = static_cast<double>(iter.indtotstaked.amount) / newstats.totalstaked.amount;

etffees_def etffeestb(_self, _self.value);
etffees feeitr;
feeitr = etffeestb.get();




double divsint = (feeitr.totalfees.amount * percgets);


struct asset divs = {int64_t (divsint), symbol ("EOSETF", 4)};

createetf(user, divs);


feesadjust_def etffeestbadj(_self, _self.value);
feesadjust feeitradj;
feeitradj = etffeestbadj.get();

//ADJUSTCRTCLM ADJUSTS TOTAL FEES WHEN NEW PERIOD STARTS.NUMBER IS POSITIVE IF MORE WAS CREATED EOSETF THAT CLAIMED.
feeitradj.adjustcrtclm.amount -= divs.amount;
etffeestbadj.set(feeitradj, _self);


feeitradj = etffeestbadj.get();

//HERE'S THE ADJUSTMENT
feeitr.totalfees.amount += feeitradj.adjustcrtclm.amount;
etffeestb.set(feeitr, _self);


feeitr = etffeestb.get();

check(feeitr.totalfees.amount >= 0, "Total fees to be distr fell below 0.");




//PERIOD STARTS WITH 0
feeitradj.adjustcrtclm.amount = 0;
etffeestbadj.set(feeitradj, _self);


//THIS IS TABLE THAT TRACKS HOW MUCH INDIVIDUAL HAS STAKED.
auto totalrow = indtotstk.find(user.value);
indtotstk.modify(totalrow,name("consortiumtt"), [&]( auto& s ){
  s.indtotstaked.amount = 0;
         });

}




//MONSTER FUNCTION
[[eosio::action]]
void rebalance(name user, uint64_t pollkey, name community)

{

require_auth( user );

//CHECK IF USER IS FUND MANAGER
approvedaccs whitetbl("consortiumlv"_n, community.value);
auto whiterow = whitetbl.find(user.value);
check(whiterow != whitetbl.end(), "Account not whitelisted.");


nrofmngtab managtbl("consortiumlv"_n, "consortiumlv"_n.value);

const auto &itermang =managtbl.get( community.value, "No poll found with such key" );


kysimustes pollstbl("consortiumlv"_n, community.value);

const auto &iter = pollstbl.get( pollkey, "No poll found with such key" );

//THIS SHOULD BE ADDED IF 2/3 HAVE TO VOTE IN ORDER TO REBALANCE
/*
if (static_cast<double>(iter.nrofvoters) / itermang.nrofmanagers < 0.656)

{
check(false, "2/3 of managers have to vote in order to rebalnce.");
}
*/

//SETTING THAT NOBODY HAS VOTED IN THE POLL.
votersnulli(community,pollkey);

  //LOOP START THAT CALCULATES NEW PERCENTAGES
  for(int i=0; i < iter.answers.size(); i++){

//CALCULATING THE NEW ALLOCATION OF TOKENS BASED ON THE VOTE RESULTS
   double newpercentage = static_cast<double>(iter.totalvote[i]) / iter.sumofallopt;

check(newpercentage == 0 || newpercentage >= 0.01, "Min token allocation % is 1.");


    auto sym = iter.answers[i];
    rebalontb rebaltab(get_self(), _self.value);
    auto existing = rebaltab.find( sym.code().raw() );
    //SAVING NEW ALLOCATION PERCENTAGE
            rebaltab.modify(existing,name("consortiumtt"), [&]( auto& s ){
              s.tokenpercnew    = newpercentage;
        });
        
         }
         //LOOP ENDED THAT CALCULATES NEW PERCENTAGES

//SETTING TOTAL FUND WORTH TO 0, NEXT LOOP CALCULATES CURRENT VALUE
totleostab eostable(_self, _self.value);
totaleosworth soloiterr;

if (eostable.exists())
{
soloiterr = eostable.get();

soloiterr.eosworth = 0;

eostable.set(soloiterr, _self);

}


rebalontb rebaltab(get_self(), _self.value);


//LOOP CALCULATING HOW MUCH TOKENS ARE WORTH IN EOS
           for (auto iter = rebaltab.begin(); iter != rebaltab.end(); iter++)
{


pairs pairtab("swap.defi"_n, "swap.defi"_n.value);

const auto &iterpair = pairtab.get(iter->pairid, "No row with such pairid" );

//CHECK DUE TO HOW DEFIBOX TABLES ARE BUILT
if (iterpair.reserve0.symbol == iter->token) {

double eosworth = iterpair.price0_last * iter->tokeninfund;

            auto existing = rebaltab.find( iter->token.code().raw() );
rebaltab.modify(existing,name("consortiumtt"), [&]( auto& s ){
            s.tokenwortheos    = eosworth;
        });

}

//CHECK DUE TO HOW DEFIBOX TABLES ARE BUILT
if (iterpair.reserve1.symbol == iter->token) {

double eosworth = iterpair.price1_last * iter->tokeninfund;

            auto existing = rebaltab.find( iter->token.code().raw());
            rebaltab.modify(existing,name("consortiumtt"), [&]( auto& s ){
            s.tokenwortheos    = eosworth;
        });

}
//CALCULATING TOTAL EOS WORTH OF TOKENS IN FUND
totleostab eostable(_self, _self.value);
totaleosworth soloiter;
soloiter = eostable.get();

soloiter.eosworth += iter->tokenwortheos;

eostable.set(soloiter, _self);


}
//END OF FIRST LOOP CALCULATING TOKEN WORTH IN EOS




//LOOP CALCULATING THE CURRENT PERCENTAGE OF TOKENS IN FUND 
 for(int i=0; i < iter.answers.size(); i++){


 totleostab eostable(_self, _self.value);
 totaleosworth soloiter;
 soloiter = eostable.get();
 
rebalontb reblatab(get_self(), _self.value);

const auto &rebapiter = reblatab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );

double tokenperold = rebapiter.tokenwortheos / soloiter.eosworth;

auto uus = reblatab.find( iter.answers[i].code().raw() );
reblatab.modify(uus,name("consortiumtt"), [&]( auto& s ){
              s.tokenperold    = tokenperold;
 });

}


//LOOP THAT SELLS TOKENS THROUGH DEFIBOX
for(int i=0; i < iter.answers.size(); i++){

rebalontb rbtab(get_self(), _self.value);

const auto &rbaliter = rbtab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );

//SELLING TOKENS IF CURRENT PERCENTAGE IS LARGER THAN NEW
if (rbaliter.tokenperold > rbaliter.tokenpercnew && rbaliter.tokenperold != 0) {

double diffpertosell =rbaliter.tokenperold -rbaliter.tokenpercnew;

double perdiff = diffpertosell /rbaliter.tokenperold;

double toselldoub =rbaliter.tokeninfund * perdiff;

struct asset tosell = {int64_t (toselldoub*rbaliter.decimals),rbaliter.token};

string memo = "swap,0," + rbaliter.strpairid;

//ACTION THAT TRIGGERS SELLING
send(_self,"swap.defi"_n, tosell, memo,rbaliter.contract);  

//SAVE AMOUNTS AFTER SELLING
//INLINE ACTION NEEDED OTHERWISE send IS EXECUTED LAST AND THUS OLD BAlANCE IS SAVED
adjusttokk(rbaliter.contract, rbaliter.token, rbaliter.decimals, rbaliter.tokenpercnew);

}

}
//END LOOP THAT SELLS TOKENS THROUGH DEFIBOX



//LOOP THAT BUYS TOKENS THROUGH DEFIBOX
for(int i=0; i < iter.answers.size(); i++){


rebalontb rebaltab(get_self(), _self.value);

const auto &rebaliter = rebaltab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );


if (rebaliter.tokenperold < rebaliter.tokenpercnew && rebaliter.tokenperold != 0) {

const auto &rebit = rebaltab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );

pairs pairtab("swap.defi"_n, "swap.defi"_n.value);

const auto &iterpair = pairtab.get(rebit.pairid, "No row with such pairid" );

double diffpertobuy = rebaliter.tokenpercnew - rebaliter.tokenperold;

double perdiff = diffpertobuy / rebaliter.tokenperold;

double eosworthtobuy = rebaliter.tokenwortheos * perdiff;

struct asset tobuy = {int64_t ((eosworthtobuy * 10000)*0.997), symbol ("EOS", 4)};

string memo = "swap,0," + rebaliter.strpairid;



//ACTION THAT TRIGGERS BUYING
send(_self,"swap.defi"_n, tobuy, memo, "eosio.token"_n);  

//SAVE AMOUNTS AFTER BUYING
//INLINE ACTION NEEDED OTHERWISE send IS EXECUTED LAST AND THUS OLD BAlANCE IS SAVED
adjusttokk(rebaliter.contract, rebaliter.token, rebaliter.decimals, rebaliter.tokenpercnew);


}


//IN CASE CURRENT % WAS 0 and NEW VOTE ADDED IT
if (rebaliter.tokenperold == 0 && rebaliter.tokenpercnew != 0) {

const auto &rebit = rebaltab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );

pairs pairtab("swap.defi"_n, "swap.defi"_n.value);

const auto &iterpair = pairtab.get(rebit.pairid, "No row with such pairid" );

totleostab eostable(_self, _self.value);
totaleosworth soloiter;

soloiter = eostable.get();
 
double eosworthtobuy = rebaliter.tokenpercnew * soloiter.eosworth;

struct asset tobuy = {int64_t (eosworthtobuy * 10000), symbol ("EOS", 4)};

string memo = "swap,0," + rebaliter.strpairid;

send(_self,"swap.defi"_n, tobuy, memo, "eosio.token"_n);  

//SAVE AMOUNTS AFTER BUYING
//INLINE ACTION NEEDED OTHERWISE send IS EXECUTED LAST AND THUS OLD BAlANCE IS SAVED
adjusttokk(rebaliter.contract, rebaliter.token, rebaliter.decimals, rebaliter.tokenpercnew);

}

}
//END LOOP THAT BUYS TOKENS THROUGH DEFIBOX





rebalontb pedetb(get_self(), _self.value);
//LOOP TO GET MIN AMOUNTS OF TOKENS TO CREATE EOSETF
    for (auto iter = pedetb.begin(); iter != pedetb.end(); iter++)
{

const auto &rebaliter = pedetb.get(iter->token.code().raw(), "No pairid for such symbol" );

//GET DESIRED PRICE FOR 1 EOSETF
etfpricetb eostable(_self, _self.value);
etfprice soloiter;
soloiter = eostable.get();

//GET HOW MUCH EOS WORTH SHOULD THAT TOKEN HAVE
double mineostokworth = iter->tokenpercnew * soloiter.one;


pairs pairtab("swap.defi"_n, "swap.defi"_n.value);

const auto &iterpair = pairtab.get(iter->pairid, "No row with such pairid" );


if (iterpair.reserve0.symbol == iter->token) {

double mintokenamt = mineostokworth / iterpair.price0_last;

struct asset minamount = {int64_t (rebaliter.decimals * mintokenamt), rebaliter.token};

auto iterviis =pedetb.find( iter->token.code().raw() );

pedetb.modify(iterviis,name("consortiumtt"), [&]( auto& s ){
              s.minamount    = minamount;
});


check(minamount.amount < 10000000000000, "Minamount too large");


}

if (iterpair.reserve1.symbol == iter->token) {

double mintokenamt = mineostokworth / iterpair.price1_last;

struct asset minamount = {int64_t (rebaliter.decimals * mintokenamt), rebaliter.token};

auto iterkuus =pedetb.find( iter->token.code().raw() );

pedetb.modify(iterkuus,name("consortiumtt"), [&]( auto& s ){
              s.minamount    = minamount;
});

check(minamount.amount < 10000000000000, "Minamount too large");


}

}
//LOOP TO GET MIN AMOUNTS CLOSED

//DELETING BASE ITER, BASE ITER IS USED TO DETERMINE THE CORRECT RATIOS WHEN CREATING EOSETF.
//EVERY REBALANCING GETS NEW BASE ITER, THE SMALLEST AMOUNT FROM ALL THE TOKENS CURRENTLY IN THE FUND. 
basetoktab basetable(_self, _self.value);
basetok baseiter;

if (basetable.exists())
{
baseiter = basetable.get(); 

basetable.remove();
}




rebalontb vitttb(get_self(), _self.value);

//LOOP TO GET THE SMALLEST VALUE IN THE BASE ITERATOR 
    for (auto iter = vitttb.begin(); iter != vitttb.end(); iter++)
{

if (iter->minamount.amount > 0)
{

basetoktab basetable(_self, _self.value);
basetok baseiter;

if (!basetable.exists()) 

{
basetable.set(baseiter, _self);
baseiter.base = iter->minamount.symbol;
basetable.set(baseiter, _self);
}

else

{
baseiter = basetable.get(); 

const auto &itrbase = vitttb.get(baseiter.base.code().raw(), "No token with such symbol." );

if (itrbase.minamount.amount > iter->minamount.amount)
{
baseiter = basetable.get(); 
baseiter.base = iter->minamount.symbol;
basetable.set(baseiter, _self);
}
}
}
}
//END LOOP TO GET THE SMALLEST VALUE IN THE BASE ITERATOR 








//LOOP TO GET NEW RATIOS (VERY CRUCIAL COMPONENT, RATIOS ENSURE THAT CORRECT AMOUNTS ARE SENT WHEN CREATING EOSETF)
  for(int i=0; i < iter.answers.size(); i++)
{

rebalontb geitb(get_self(), _self.value);


const auto &rebaliter = geitb.get(iter.answers[i].code().raw(), "No token with such symbol." );

basetoktab basetable(_self, _self.value);
basetok baseiter;

baseiter = basetable.get(); 

const auto &itrbase = geitb.get(baseiter.base.code().raw(), "No token with such symbol." );

double ratio = static_cast<double>(rebaliter.minamount.amount) / itrbase.minamount.amount;

auto iterseitse = geitb.find( iter.answers[i].code().raw() );

geitb.modify(iterseitse,name("consortiumtt"), [&]( auto& s ){
              s.ratio    = ratio;
});

}
//LOOP TO GET NEW RATIOS CLOSED


//SET SIZE *NUBMBER OF TOKENS IS THE FUND TO ZERO. SIZE NUMBER OF TOKENS IN FUND NEEDED WHEN CREATING EOSETF. 
//EOSETF IS ONLY ISSUED IF NUMBER OF DIFFERENT TOKENS YOU ARE SENDING IN EQUALS TO THE NUMBER OF TOKENS CURRENTLY IN THE FUND. 
etfsizetab sizetable(_self, _self.value);
etfsize soloiter;

soloiter = sizetable.get();
soloiter.size = 0;
sizetable.set(soloiter, _self);



//LOOP TO GET THE SIZE (NUBMBER OF TOKENS IS THE FUND)
rebalontb vasaktb(get_self(), _self.value);

 for (auto iter = vasaktb.begin(); iter != vasaktb.end(); iter++)
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
//END LOOP TO GET THE SIZE *NUBMBER OF TOKENS IS THE FUND 


}
//REBALANCE FUNCTION END




//FOR TESTING, BUT MAYBE GOOD TO HAVE JUST IN CASE THERE'S A BUG.
[[eosio::action]]
void modddtokens(vector <double> tokeninfund,
vector <asset> minamount, vector <symbol> token)

{

  require_auth ( _self);


for (size_t i = 0; i < tokeninfund.size(); ++i)

  {

     rebalontb rebaltab(_self, _self.value);
      auto existing = rebaltab.find( token[i].code().raw() );
         

const auto& st = *existing;

            rebaltab.modify(st , name("consortiumtt"), [&]( auto& s ){
             s.tokeninfund = tokeninfund[i];
             s.minamount = minamount[i];
      
        });
        }

}





//ADDS TOKEN TO THE FUND. ALLOCATION 0. SO FUND MANAGERS HAVE TO VOTE TO ACTUALLY INCLUDE THE TOKEN IN THE FUND. ALSO ON_NOTIFY IS REQUIRED FOR EACH ADDED TOKEN.
[[eosio::action]]
void addtokuz(vector <double> tokeninfund, vector <double> tokenwortheos, vector <double> tokenperold, vector <double> tokenpercnew, vector <int64_t> decimals, vector <uint64_t> pairid,
vector <string>  strpairid, vector <symbol> token, vector <name> contract, vector <double> ratio, vector <asset> minamount, vector <uint64_t> totalvote, vector <string> image, name community, uint64_t pollkey)
{

  require_auth ( _self);

  for (size_t i = 0; i < pairid.size(); ++i)

  {

     rebalontb rebaltab(get_self(), _self.value);
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
             s.image = image[i];
      
        });

                                   }

     if(existing!=rebaltab.end() ) {
check(false, "Token already added.");

                                   }


 }


sendsymtotvot(token,totalvote,community, pollkey);

}




//POSSIBILITY TO PAUSE CREATION AND REDEMPTION IN CASE OF BUG / EMERGENCY
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





//SAVES NEW TOKEN BALANCE (USED IN NEXT REBALANCING)
[[eosio::action]]
void adjusttok( name contract, symbol token, int64_t decimals, double tokenpercnew  )
{

//KUI KÕIK SOLD SIIS TA FJUKOF KUNA SEE L2heb nulli ehk siis ei saa querida midagi.

rebalontb rebaltab(get_self(), _self.value);
auto iterkolm = rebaltab.find( token.code().raw() );

if (tokenpercnew != 0) 
{
accounts from_acnts( contract, _self.value );
const auto& from = from_acnts.get(token.code().raw(), "Fjukof" );

//NO need for double actually, UINT would be more precise.
double afterbuyingamt =  static_cast<double>(from.balance.amount) / decimals;


rebaltab.modify(iterkolm,name("consortiumtt"), [&]( auto& s ){
              s.tokeninfund    = afterbuyingamt;
});

}

else {

rebaltab.modify(iterkolm,name("consortiumtt"), [&]( auto& s ){
              s.tokeninfund    = 0;
});
}


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
   
}

/*
[[eosio::on_notify("token.defi::transfer")]]
void minecapture (name from, name to, asset quantity, std::string memo){
 
if (from == "mine2.defi"_n )
{
check(false,"pede");
}

}
*/


//ALL ON_NOTIFY ARE USED WHEN CREATING EOSETF, TO CHECK IF CORRECT AMOUNTS AND TOKENS ARE BEING SENT IN
[[eosio::on_notify("tethertether::transfer")]]
void issueetfusdt (name from, name to, asset quantity, std::string memo){
 
if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}

}


[[eosio::on_notify("dappservices::transfer")]]
void issueetfdapp (name from, name to, asset quantity, const string memo)

{ 
  
//dappfund1 sends dividends, hence is being ignored. 
if (from != "thedappfund1"_n && from != "swap.defi"_n )


{

savetokens(from, quantity,to);

}

}

[[eosio::on_notify("effecttokens::transfer")]]
void issueetfefx (name from, name to, asset quantity, std::string memo){
 
if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}

}


[[eosio::on_notify("core.ogx::transfer")]]
void issueetfogx (name from, name to, asset quantity, std::string memo){

if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}
}


[[eosio::on_notify("everipediaiq::transfer")]]
void issueetfiq (name from, name to, asset quantity, std::string memo){

if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}
}


[[eosio::on_notify("vig111111111::transfer")]]
void issueetfvig (name from, name to, asset quantity, std::string memo){

if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}
}

//SOME ADJUSTMENTS HERE BECAUSE DEFIBOX HAS REWARDS FOR SWAPPING
[[eosio::on_notify("token.defi::transfer")]]
void issueetfbox (name from, name to, asset quantity, std::string memo){
     
if (from != "swap.defi"_n && from != "mine2.defi"_n)
{
savetokens(from, quantity,to);
}

if (from == "mine2.defi"_n )
{

double quandoub = static_cast<double>(quantity.amount) / 1000000;

rebalontb rebaltab(get_self(), _self.value);

const auto &rebaliter = rebaltab.get(quantity.symbol.code().raw() , "No such token in rebal table" );

if (rebaliter.tokeninfund != 0)

{

auto iterkolm = rebaltab.find(quantity.symbol.code().raw() );
rebaltab.modify(iterkolm,name("consortiumtt"), [&]( auto& s ){
              s.tokeninfund    += quandoub;
        });

}

}

}

[[eosio::on_notify("dadtoken1111::transfer")]]
void issueetfdad (name from, name to, asset quantity, std::string memo){
     
if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}
}

[[eosio::on_notify("pizzatotoken::transfer")]]
void issueetfpizza (name from, name to, asset quantity, std::string memo){
     
if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}
}

[[eosio::on_notify("chexchexchex::transfer")]]
void issueetfchex (name from, name to, asset quantity, std::string memo){
     
if (from != "swap.defi"_n)
{
savetokens(from, quantity,to);
}

}


[[eosio::on_notify("token.newdex::transfer")]]
void issueetfdex (name from, name to, asset quantity, std::string memo){
     
if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}
}

[[eosio::on_notify("eosiotptoken::transfer")]]
void issueetftpt (name from, name to, asset quantity, std::string memo){
     
if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}
}


[[eosio::on_notify("emanateoneos::transfer")]]
void issueetfemt (name from, name to, asset quantity, std::string memo){
     
if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}
}

[[eosio::on_notify("minedfstoken::transfer")]]
void issueetfdfs (name from, name to, asset quantity, std::string memo){
     
if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}
}



private:


//FUNCTION TO REDEEM TOKENS  
    void refund_tokens_back(name from, name to, asset quantity, std::string memo) 
    {

    check (quantity.symbol == symbol("EOSETF", 4), "Token with wrong symbol.");

    check (quantity.amount >= 10000, "Can't redeem less than 1 EOSETF" );

    check (quantity.amount <= 2000000, "Can't more than 200 EOSETF" );

    pauseornot();


    rebalontb reftab(get_self(), _self.value);


   refundratetb eostable(_self, _self.value);
   refundrate soloiter;

  
    soloiter = eostable.get();
 

    for (auto iter = reftab.begin(); iter != reftab.end(); iter++)
{

//REDEEMS ONLY IF CURRENTLY IN THE FUND
if (iter->tokenpercnew > 0)

{
     struct asset refundasset = {int64_t ((quantity.amount * iter->minamount.amount*soloiter.rate)/10000), iter->token};

     send(to, from, refundasset, memo, iter->contract);  

//DEDUCTING FROM REBALTAB TOKENSINFUND
rebalontb rebaltab(get_self(), _self.value);

const auto &rebaliter = rebaltab.get(iter->token.code().raw() , "No such token in rebal table" );

double deductfund = static_cast<double>((quantity.amount * iter->minamount.amount*soloiter.rate)/10000)/iter->decimals;


auto existing = rebaltab.find( iter->token.code().raw() );
rebaltab.modify(existing,name("consortiumtt"), [&]( auto& s ){
            s.tokeninfund    -= deductfund;
        });




}

}

}









void sub_balance( name owner, asset value ) 
{
   accounts from_acnts( _self, owner.value );
   const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found in accounts" );

   

check( from.balance >= ( value ), "sub_balance: from.balance overdrawn balance" );

auto sym = symbol ("CETF", 4);

//IGNORING EOSETF SUBSTRACTIONS
if (value.symbol == sym)

{

perzonstkd personstktbl(_self, owner.value);

//THIS LOOP CHECKS HOW MUCH HAS USER STAKED IN TOTAL
for (auto iter = personstktbl.begin(); iter != personstktbl.end(); iter++)
{


perstotlskd perstottb(_self, _self.value);
auto totrow = perstottb.find(owner.value);

     if(totrow!=perstottb.end() ) {
     perstottb.modify(totrow,name("consortiumtt"), [&]( auto& s ){
             s.indtotstaked += iter->staked;
         });
}

}


perstotlskd perstotkaks(_self, _self.value);
auto totrowkaks = perstotkaks.find(owner.value);

if(totrowkaks!=perstotkaks.end() ) {

const auto &tra =perstotkaks.get(owner.value, "No totstaked for user" );

check( from.balance.amount - tra.indtotstaked.amount >= ( value.amount ), "sub_balance: unstake CETF to transfer" );

//SETTING TOTAL STAKE TO ZERO AGAIN IN ORDER FOR THE CHECK TO HAPPEN NEXT TIME AGAIN IF TRANSFERRED
perstotkaks.modify(totrowkaks,name("consortiumtt"), [&]( auto& s ){
  s.indtotstaked.amount = 0;
         });

}


}//CLOSING IF SYM == CETF
  

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

        statstable.modify( st, name("consortiumtt"), [&]( auto& s ) {
            s.supply += quantity;
        });
        
        
      
       add_balance( owner, quantity, name("consortiumtt"));

       //add_balance( owner, quantity, owner);
    }


void pauseornot( ) {

pausetab pauztab(_self, _self.value);
pausetabla iter;

iter = pauztab.get();

check(iter.ispaused, "Creation and redemption is currently halted.");
}



//CRUCIAL FUNCTION THAT CHECKS IF CORRECT AMOUNTS OF TOKENS ARE SENT IN
//ON_NOTIFY CAPTURES ALL THE INCOMING TOKENS AND SAVES THEM INTO USERITOKANS TABLE
void savetokens( name from, asset quantity, name to )
    {
if (to  != "consortiumtt"_n) return;

pauseornot();

  rebalontb sinput(get_self(), _self.value);
      auto secinput = sinput.find( quantity.symbol.code().raw() );

    useritokans input(get_self(), from.value);
      auto newinput = input.find( quantity.symbol.code().raw() );



 check (quantity.symbol == secinput->token, "Incorrect symbol.");

//SHOULD THERE BE CHECK FOR MAX AMOUNT?

 check (quantity.amount >= secinput->minamount.amount, "Minimum creation threshold is 1 EOSETF.");

//THIS NEEDED IN ORDER TO SAVE ONLY THE TOKENS THAT ARE INCLUDED IN THE FUND
if (secinput->tokenpercnew > 0) 

{

     if( newinput == input.end() ) {
         input.emplace( name("consortiumtt"), [&]( auto& a ){
            a.token= quantity;
            a.ratio = secinput->ratio;
         });

     }
         else{
            input.modify(newinput,name("consortiumtt"), [&]( auto& a ){
            a.token += quantity;
            a.ratio = secinput->ratio;
        });
      }

}

checkratuus (from, quantity);
}


void checkratuus( name from, asset quantity )
    {

basetoktab basetable(_self, _self.value);
basetok baseiter;

baseiter = basetable.get();    

useritokans input(get_self(), from.value);

auto size = std::distance(input.cbegin(),input.cend());

etfsizetab sizetable(_self, _self.value);
etfsize soloiter;

soloiter = sizetable.get();

//if size is smaller will not issue CETF/EOSETF, just saves the token values in the table. 
if (size == soloiter.size ) {



const auto& basetokrow = input.find(baseiter.base.code().raw() );

//LOOP THAT CHECKS THE RATIO FOR EACH TOKEN
for (auto iter = input.begin(); iter != input.end();)
{



//ADDING TOKENS TO FUND, NEEDED FOR REBALANCING PURPOSES. 
rebalontb rebaldab(get_self(), _self.value);

const auto &rebaliter = rebaldab.get(iter->token.symbol.code().raw() , "No such token in rebal table" );


double addtofund = static_cast<double>(iter->token.amount) / rebaliter.decimals;


auto otsiexisting = rebaldab.find(iter->token.symbol.code().raw() );
rebaldab.modify(otsiexisting,name("consortiumtt"), [&]( auto& s ){
            s.tokeninfund    += addtofund;
        });



//CHECKING IF RATIOS CORRECT
check (iter->token.amount != 0, "Doggy Afuera!");

//THIS ONE CRUCIAL, CHECKS IF INCOMING TOKENS PRODUCE THE PREDETERMINED RATIO (CALCULATED DURING THE REBELANCE FUNCTION). IF PRECISELY THE SAME (RATIO IS A DOUBLE) THEN 
check ((static_cast<double>(iter->token.amount) / basetokrow->token.amount == iter->ratio), "Incorrect token ratios.");


input.erase(iter++);


}



rebalontb rebaltab(get_self(), _self.value);
    auto iteraator = rebaltab.find( baseiter.base.code().raw());


struct asset numberofetfs = {int64_t ((basetokrow->token.amount/iteraator->minamount.amount)*10000), symbol ("EOSETF", 4)};



feesadjust_def etffeestb(_self, _self.value);
feesadjust feeitr;
etffeestb.set(feeitr, _self);

refundratetb eostable(_self, _self.value);
refundrate soloiter;
soloiter = eostable.get();

//ADD TO THE SINGLETON THAT CALCULATES HOW MUCH FEE WAS ACCUMULATED DURING A PERIOD
feeitr.adjustcrtclm.amount += numberofetfs.amount * (1-soloiter.rate);
etffeestb.set(feeitr, _self);

//ISSUE ETF
createetf(from, numberofetfs );

auto sym = symbol ("CETF", 4);


stats statstable( _self, sym.code().raw() );
auto existing = statstable.find( sym.code().raw() );
const auto& st = *existing;

//SOME CETF ISSUANCE FOR CREATING EOSETF, TOUCH OF SATOSHI, HIS HALVING METHOD FOR BTC
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


 void sendsymtotvot(vector <symbol> sym, vector <uint64_t> totalvote, name community, uint64_t pollkey) {
    
      action(
      permission_level{get_self(),"active"_n},
      "consortiumlv"_n,
      "addtok"_n,
      std::make_tuple(sym,totalvote,community,pollkey)
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

 void send(name from, name to, asset quantity, std::string memo, name contract) {
    
      action(
      permission_level{get_self(),"active"_n},
      contract,
      "transfer"_n,
      std::make_tuple(from,to,quantity,memo)
    ).send();
  };


 void adjusttokk( name contract, symbol token, int64_t decimals, double tokenpercnew ) {

      action(
      permission_level{get_self(),"active"_n},
      _self,
      "adjusttok"_n,
      std::make_tuple(contract,token,decimals, tokenpercnew)
    ).send();
  };






};




