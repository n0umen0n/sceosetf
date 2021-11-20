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



struct [[eosio::table]] usertokens {
         asset    token;
         double ratio;
        
         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };

      typedef eosio::multi_index< name("usertokens"), usertokens > useritokans;

/*
struct [[eosio::table]] vitt {
         asset    token;
        
         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };

      typedef eosio::multi_index< name("vitt"), vitt > piidritab;


struct [[eosio::table]] lits {
         name    from;
        
         uint64_t primary_key()const { return from.value; }
      };

      typedef eosio::multi_index< name("lits"), lits > litstab;

*/

//STAKING TABLES
/*

TABLE perstotalstkd {

    name staker 

    asset indtotstaked;

    auto primary_key() const { return staker.value; }

    };

    typedef eosio::multi_index<name("perstotalstkd"), perstotalstkd > perstotlstkd;


TABLE personstaked {

    uint64_t id;

    asset staked;

    time_point_sec staketime;

    auto primary_key() const { return id; }

    };

    typedef eosio::multi_index<name("personstaked"), personstaked > personstkd;




TABLE claimtime {

    //time_point_sec claimtime;

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






*/







TABLE etffees {

  asset totalfees = {int64_t (00000), symbol ("EOSETF", 4)};
};
typedef eosio::singleton<"etffees"_n, etffees> etffees_def;


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

TABLE etfprice{

  double one;
};
typedef eosio::singleton<"etfprice"_n, etfprice> etfpricetb;

/*
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

typedef eosio::multi_index<"rebalancedt"_n, rebalancedt> rebalontb;
*/

/*

TABLE rebalrsk {

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

typedef eosio::multi_index<"rebalrsk"_n, rebalrsk> rebalontb;
*/


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



//CODE FOR REBALANCING

//FINISH TABLES DEFIBOX AND TEST IF WORK. 

/*
TABLE nrofmanagers {

uint64_t nrofmanagers;

auto primary_key() const { return nrofmanagers; }

    };

typedef eosio::multi_index<name("nrofmanagers"), nrofmanagers > nrofmanagtbl;
*/


TABLE mngtab {

uint64_t nrofmanagers;

name community;

auto primary_key() const { return community.value; }

    };

typedef eosio::multi_index<name("mngtab"), mngtab > nrofmngtab;


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


/*

TABLE testdoub {

name accounts;

double theratio;

auto primary_key() const { return accounts.value; }
    };

typedef eosio::multi_index<name("testdoub"), testdoub > testdoubtb;

*/



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



/*
[[eosio::action]]
void doubletest(asset one, asset two, name user)
{


double theratio = static_cast<double>(one.amount) / two.amount;


     testdoubtb rebaltab(get_self(), _self.value);
      auto existing = rebaltab.find( user.value );
         

     if(existing==rebaltab.end() ) {
         rebaltab.emplace( _self, [&]( auto& s ) {
             s.theratio    = theratio;
             s.accounts    = user;      
        });

                                   }

     if(existing!=rebaltab.end() ) {
rebaltab.modify(existing,name("consortiumtt"), [&]( auto& s ){
 s.theratio    = theratio;
         });
        

                                   }


}

*/
/*

[[eosio::action]]
void rebaltjest(name user, uint64_t pollkey, name community)

{


kysimustes pollstbl("consortiumlv"_n, community.value);

const auto &iter = pollstbl.get( pollkey, "No poll found with such key" );

  for(int i=0; i < iter.answers.size(); i++){

//CALCULATING THE NEW ALLOCATION OF TOKENS
   double newpercentage = static_cast<double>(iter.totalvote[i]) / iter.sumofallopt;


//siin vaja et stringide asemel oleks symboolkad...
//inline action mis trigerdab consortiumlvs addition of new answer nii int kui ka sym
    auto sym = iter.answers[i];
    rebalontb rebaltab(get_self(), _self.value);
    auto existing = rebaltab.find( sym.code().raw() );
    
            rebaltab.modify(existing,name("consortiumtt"), [&]( auto& s ){
              s.tokenpercnew    = newpercentage;
        });
        
         }
         //LOOP ENDED THAT CALCULATES NEW PERCENTAGES


totleostab eostable(_self, _self.value);
totaleosworth soloiterr;

if (eostable.exists())
{
soloiterr = eostable.get();

soloiterr.eosworth = 0;
}


    rebalontb rebaltab(get_self(), _self.value);


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
 
rebalontb rebaltab(get_self(), _self.value);

//siia vaja see code raw
const auto &rebaliter = rebaltab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );

double tokenperold = rebaliter.tokenwortheos / soloiter.eosworth;

check(false, soloiter.eosworth);

auto existing = rebaltab.find( iter.answers[i].code().raw() );
rebaltab.modify(existing,name("consortiumtt"), [&]( auto& s ){
              s.tokenperold    = tokenperold;
 });
}


}

*/






[[eosio::action]]
void unstakecetf(name user, vector <asset> quantity, vector <uint64_t> id){


require_auth ( user );

    auto sym = quantity.symbol.code();
    stats statstable( _self, sym.raw() );
    const auto& st = statstable.get( sym.raw() );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must ustake positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch while staking" );

    accounts from_acnts( _self, staker.value );
   const auto& from = from_acnts.get( quantity.symbol.code().raw(), "no balance object found" );



  for(int i=0; i < iter.quantity.size(); i++){

personstkd personstktbl(_self, user.value);

auto userrow = personstktbl.find(id[i]);

const auto &iterone =personstktbl.get(id[i], "No such staking ID(1)." );

check(iterone.staked.amount >= quantity.amount[i], "Unstaking too much CETF.");

  personstktbl.modify(userrow,name("consortiumtt"), [&]( auto& s ){
             s.staked.amount -= quantity.amount[i];
         });


const auto &itertwo =personstktbl.get(id[i], "No such staking ID(2)." );

if (itertwo.staked.amount == 0) {

   personstktbl.erase(usersrow);

}

}

totalstk_def totalstktbl(_self, _self.value);
totalstk newstats;

newstats = totalstktbl.get();

  newstats.totalstaked.amount -= quantity.amount;
  totalstktbl.set(newstats, _self);


}










[[eosio::action]]
void stakecetf(name user, asset quantity, uint64_t id){


require_auth ( user );

    auto sym = quantity.symbol.code();
    stats statstable( _self, sym.raw() );
    const auto& st = statstable.get( sym.raw() );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must stake positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch while staking" );

    accounts from_acnts( _self, staker.value );
   const auto& from = from_acnts.get( quantity.symbol.code().raw(), "no balance object found" );

//ADDING TO TOTAL INDIVIDUALS TAKED AMOUNT


//SIIA ALGUSES SALVESTA TEMA INDSTK TABELISSE ID JA STAKED AMOUNT, SIIS AINT ALUMINE CHECK TOIMIB!


check( from.balance.amount >= quantity.amount, "Staking more CETF than you have." );

personstkd personstktbl(_self, user.value);
auto userrow = personstktbl.find(id);
if(userrow==personstktbl.end() ) {
personstktbl.emplace( _self, [&]( auto& s ) {
             s.id    = id;                            
             s.staked    = quantity;                            
             s.staketime    = current_time_point();                            


}
     if(userrow!=personstktbl.end() ) {
check(false, "This ID already in use, please try staking again.");
     }






   
for (auto iter = personstktbl.begin(); iter != personstktbl.end(); iter++)
{


perstotlstkd perstottb(_self, _self.value);
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


//KAS SEDA ON VAJA UUESTI DECLARIDA
perstotlstkd indtotstk(_self, _self.value);
const auto &iter =indtotstk.get(user.value, "Individual has not staked." );

check( iter.indtotstaked.amount >= quantity.amount, "Trying to stake more than available CETF." );


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



//SIIS KUI ON getdiv periood siis avanegu teine periood, selle p'eva jooksul hakkab kogunema sinna. 
[[eosio::action]]
void getdiv(name user)

{

divperiod_def divpertb(_self, _self.value);
divper divperiod;

divper = divpertb.get();


if (divper.periodstart + seconds(604800) < current_time_point()) {

//period pole läbi 

//siis checki kas user juba claiminud, seda saab vaadata tema claim tabelist. kas current claim period matchib tema claim periodiga

//kui matchib siis viska error et on claiminud

//kui ei ole pane tabelisse mis perioodil on claiminud


claimtimetb claimtab(_self, _self.value);
auto claimrow = claimtab.find(user.value);

if(claimrow==claimtab.end() ) {
claimtab.emplace( _self, [&]( auto& s ) {
             s.claimperiod    = divper.claimperiod; 
             s.user = user;                           
         });
}
     if(claimrow!=claimtab.end() ) {

const auto& claimiter = claimtab.get( user.value, "User has not staked nah" );

check(claimiter.claimperiod != divpe.claimperiod, "Tra sa uuesti claimid tont");

}

else {


//divpertb.set(Config{true, 172}, _self);

  divper.periodstart = current_time_point();
  divper.claimperiod += 1;
  divpertb.set(divper, _self);

if(claimrow==claimtab.end() ) {
claimtab.emplace( _self, [&]( auto& s ) {
             s.claimperiod    = divper.claimperiod; 
             s.user = user;                           
         });
}
     if(claimrow!=claimtab.end() ) {

 claimtab.modify(claimrow,name("consortiumtt"), [&]( auto& s ){
             s.claimperiod    = divper.claimperiod;             
         });
}

}

//ELSE KUI AEG UUEKS PERIOODIKS, siis periodstart tuleb panna current time point ja incrementida nrofperiod ühevõrra
//ja lisa tema tabelisse uus row et ta on selle periodi nr. emplace or modify. 



//siis vaata kas user tabelis on value olemas, kui on 


//

//Check that user has staked at least for a week. But what if he added to the stake. Scoped by user and loop through his answers. 

personstkd personstktbl(_self, user.value);

//CHECK DOES NOT EQUAL END
//const auto& checkiter = personstktbl.get( staker.value, "User has not staked" );
//auto whiterow = personstktbl.find(user.value);
//check(whiterow != whitetbl.end(), "Account not whitelisted.");

//check what happens if loop through table which does not exist, kas viskab siis errori?????
 for (auto iter = personstktbl.begin(); iter != personstktbl.end(); iter++)
{


//checki kas on stakitud rohkem kui seitse päeva tagasi.
if (iter->staketime + seconds(604800) < current_time_point()) {

//kas selle saaks panna loopi yless proovi
//CALCULATING TOTAL STAKED BY USER
perstotlstkd perstottb(_self, _self.value);
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


//

totalstk_def totalstktbl(_self, _self.value);
totalstk newstats;

newstats = totalstktbl.get();

//KAS UUESTI VAJA
perstotlstkd indtotstk(_self, _self.value);
const auto &iter =indtotstk.get(user.value, "Individual has not staked." );


double percgets = static_cast<double>(iter.indtotstaked.amount) / newstats.totalstaked.amount;


//
etffees_def etffeestb(_self, _self.value);
totalfee feeitr;

feeitr = etffeestb.get();

//VAATA ET SIIN TA PERCGETSI INTiks ei teeks
int64_t divsint = feeitr.totalfees.amount * percgets;

struct asset divs = {int64_t (divsint), symbol ("EOSETF", 4)};

createetf(from, divs);


auto totalrow = indtotstk.find(user.value);
indtotstk.modify(totalrow,name("consortiumtt"), [&]( auto& s ){
  s.indtotstaked = 0;
         });

}





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
/*
if (static_cast<double>(iter.nrofvoters) / itermang.nrofmanagers < 0.656)

{
check(false, "2/3 of managers have to vote in order to rebalnce.");
}
*/
votersnulli(community,pollkey);



//VAJA VIIA NULLI NROFVOTERS p2rast rebalancingi.
//vaata mis toimub sum of all optionsiga. 


  for(int i=0; i < iter.answers.size(); i++){

//CALCULATING THE NEW ALLOCATION OF TOKENS
   double newpercentage = static_cast<double>(iter.totalvote[i]) / iter.sumofallopt;

check(newpercentage == 0 || newpercentage > 0.01, "Min token allocation % is 1.");


//siin vaja et stringide asemel oleks symboolkad...
//inline action mis trigerdab consortiumlvs addition of new answer nii int kui ka sym
    auto sym = iter.answers[i];
    rebalontb rebaltab(get_self(), _self.value);
    auto existing = rebaltab.find( sym.code().raw() );
    
            rebaltab.modify(existing,name("consortiumtt"), [&]( auto& s ){
              s.tokenpercnew    = newpercentage;
        });
        
         }
         //LOOP ENDED THAT CALCULATES NEW PERCENTAGES


totleostab eostable(_self, _self.value);
totaleosworth soloiterr;

if (eostable.exists())
{
soloiterr = eostable.get();

soloiterr.eosworth = 0;

eostable.set(soloiterr, _self);

}


rebalontb rebaltab(get_self(), _self.value);


//CALCULATING HOW MUCH TOKENS ARE WORTH IN EOS
           for (auto iter = rebaltab.begin(); iter != rebaltab.end(); iter++)
{


pairs pairtab("swap.defi"_n, "swap.defi"_n.value);

const auto &iterpair = pairtab.get(iter->pairid, "No row with such pairid" );

//CHECK DUE TO HOW DEFIBOX TABLES ARE BUILT
//if (iterpair.reserve0.symbol == iter->token.symbol) {
//if iter->EOS EOS then eosworth = 1 x tokeninfund
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
//VB ERRROR
//CALCULATING TOTAL EOS WORTH OF TOKENS IN FUND
totleostab eostable(_self, _self.value);
totaleosworth soloiter;
soloiter = eostable.get();

soloiter.eosworth += iter->tokenwortheos;

eostable.set(soloiter, _self);


}
//END OF FIRST LOOP CALCULATING TOKEN WORTH IN EOS




//LOOP CALCULATING THE OLD PERCETAGE OF TOKENS IN FUND 
//AND SELLING  / BUYING TOKENS FROM DEFIBOX
 for(int i=0; i < iter.answers.size(); i++){


 totleostab eostable(_self, _self.value);
 totaleosworth soloiter;
 soloiter = eostable.get();
 
rebalontb reblatab(get_self(), _self.value);

//siia vaja see code raw
const auto &rebapiter = reblatab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );

double tokenperold = rebapiter.tokenwortheos / soloiter.eosworth;


auto uus = reblatab.find( iter.answers[i].code().raw() );
reblatab.modify(uus,name("consortiumtt"), [&]( auto& s ){
              s.tokenperold    = tokenperold;
 });

}



for(int i=0; i < iter.answers.size(); i++){

rebalontb rbtab(get_self(), _self.value);

const auto &rbaliter = rbtab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );

//SELLING TOKENS
if (rbaliter.tokenperold >rbaliter.tokenpercnew && rbaliter.tokenperold != 0) {

double diffpertosell =rbaliter.tokenperold -rbaliter.tokenpercnew;

double perdiff = diffpertosell /rbaliter.tokenperold;

double toselldoub =rbaliter.tokeninfund * perdiff;

////check(false, toselldoub*rebaliter.decimals);

struct asset tosell = {int64_t (toselldoub*rbaliter.decimals),rbaliter.token};

string memo = "swap,0," + rbaliter.strpairid;


//ACTION THAT TRIGGERS SELLING
send(_self,"swap.defi"_n, tosell, memo,rbaliter.contract);  


//ADJUSTING TOKENS IN FUND
auto iterkaks =rbtab.find( iter.answers[i].code().raw() );
rbtab.modify(iterkaks,name("consortiumtt"), [&]( auto& s ){
              s.tokeninfund    -= toselldoub;

});
}
}




for(int i=0; i < iter.answers.size(); i++){


rebalontb rebaltab(get_self(), _self.value);

//siia vaja see code raw
const auto &rebaliter = rebaltab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );

//BUYING TOKENS if tokenpercnew larger than 5%?
if (rebaliter.tokenperold < rebaliter.tokenpercnew && rebaliter.tokenperold != 0) {

const auto &rebit = rebaltab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );

pairs pairtab("swap.defi"_n, "swap.defi"_n.value);

const auto &iterpair = pairtab.get(rebit.pairid, "No row with such pairid" );

double diffpertobuy = rebaliter.tokenpercnew - rebaliter.tokenperold;

double perdiff = diffpertobuy / rebaliter.tokenperold;

double eosworthtobuy = rebaliter.tokenwortheos * perdiff;

struct asset tobuy = {int64_t (eosworthtobuy * 10000), symbol ("EOS", 4)};

string memo = "swap,0," + rebaliter.strpairid;

send(_self,"swap.defi"_n, tobuy, memo, "eosio.token"_n);  

//ADJUST TOKENS IN FUND BASED THE PRICE AFTER BUYING 
if (iterpair.reserve0.symbol == iter.answers[i]) {

double newprice = (static_cast<double>(tobuy.amount + iterpair.reserve1.amount) / 10000)  / (static_cast<double>(iterpair.reserve0.amount) / rebaliter.decimals);

//check (false, newprice * 100000000);

double tokensbought = ((static_cast<double>(tobuy.amount) / 10000) * 0.997) / newprice;

//double tokensbought = ((static_cast<double>(tobuy.amount) / 10000) * 0.997) * iterpair.price1_last;

//check (false, newprice * 100000000);

auto iterkolm = rebaltab.find( iter.answers[i].code().raw() );
rebaltab.modify(iterkolm,name("consortiumtt"), [&]( auto& s ){
              s.tokeninfund    += tokensbought;

});

//const auto &rebitt = rebaltab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );
//check (false, rebitt.tokenperold * 1000000);

}


//ADJUST TOKENS IN FUND BASED THE PRICE AFTER BUYING 
if (iterpair.reserve1.symbol == iter.answers[i]) {

//double tokensbought = ((static_cast<double>(tobuy.amount) / 10000) * 0.997) * iterpair.price0_last;


double newprice = (static_cast<double>(tobuy.amount + iterpair.reserve0.amount) / 10000) / (static_cast<double>(iterpair.reserve1.amount) / rebaliter.decimals);

//check (false, newprice * 100000000);

double tokensbought = ((static_cast<double>(tobuy.amount) / 10000) * 0.997) / newprice;


auto iterneli = rebaltab.find( iter.answers[i].code().raw() );
rebaltab.modify(iterneli,name("consortiumtt"), [&]( auto& s ){
              s.tokeninfund    += tokensbought;
});

}


}







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

//ADJUST TOKENS IN FUND BASED THE PRICE AFTER BUYING 
if (iterpair.reserve0.symbol == iter.answers[i]) {

double newprice = (static_cast<double>(tobuy.amount + iterpair.reserve1.amount) / 10000)  / (static_cast<double>(iterpair.reserve0.amount) / rebaliter.decimals);

//check (false, newprice * 100000000);

double tokensbought = ((static_cast<double>(tobuy.amount) / 10000) * 0.997) / newprice;

//double tokensbought = ((static_cast<double>(tobuy.amount) / 10000) * 0.997) * iterpair.price1_last;

//check (false, newprice * 100000000);

auto iterkolm = rebaltab.find( iter.answers[i].code().raw() );
rebaltab.modify(iterkolm,name("consortiumtt"), [&]( auto& s ){
              s.tokeninfund    += tokensbought;

});

//const auto &rebitt = rebaltab.get(iter.answers[i].code().raw(), "No pairid for such symbol" );
//check (false, rebitt.tokenperold * 1000000);

}


//ADJUST TOKENS IN FUND BASED THE PRICE AFTER BUYING 
if (iterpair.reserve1.symbol == iter.answers[i]) {

//double tokensbought = ((static_cast<double>(tobuy.amount) / 10000) * 0.997) * iterpair.price0_last;


double newprice = (static_cast<double>(tobuy.amount + iterpair.reserve0.amount) / 10000) / (static_cast<double>(iterpair.reserve1.amount) / rebaliter.decimals);

//check (false, newprice * 100000000);

double tokensbought = ((static_cast<double>(tobuy.amount) / 10000) * 0.997) / newprice;


auto iterneli = rebaltab.find( iter.answers[i].code().raw() );
rebaltab.modify(iterneli,name("consortiumtt"), [&]( auto& s ){
              s.tokeninfund    += tokensbought;
});

}


}




}


rebalontb pedetb(get_self(), _self.value);


//LOOP TO GET MIN AMOUNTS 
    for (auto iter = pedetb.begin(); iter != pedetb.end(); iter++)
{

//kas seda on vaja? pigem ei
const auto &rebaliter = pedetb.get(iter->token.code().raw(), "No pairid for such symbol" );

etfpricetb eostable(_self, _self.value);
etfprice soloiter;
soloiter = eostable.get();

double mineostokworth = iter->tokenpercnew * soloiter.one;

// THOSE MAYBE NOT NEEDED
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

//LOOP TO GET MIN AMOUNTS CLOSED
}


//DELETING BASE ITER
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







//LOOP TO GET NEW RATIOS
  for(int i=0; i < iter.answers.size(); i++)
{

rebalontb geitb(get_self(), _self.value);


const auto &rebaliter = geitb.get(iter.answers[i].code().raw(), "No token with such symbol." );

basetoktab basetable(_self, _self.value);
basetok baseiter;

//baseiter = basetable.get(); 
/*
if (!basetable.exists() && rebaliter.tokenpercnew > 0) 

{

basetable.set(baseiter, _self);
baseiter.base = rebaliter.minamount.symbol;
basetable.set(baseiter, _self);


}
*/

baseiter = basetable.get(); 


const auto &itrbase = geitb.get(baseiter.base.code().raw(), "No token with such symbol." );

double ratio = static_cast<double>(rebaliter.minamount.amount) / itrbase.minamount.amount;

auto iterseitse = geitb.find( iter.answers[i].code().raw() );

geitb.modify(iterseitse,name("consortiumtt"), [&]( auto& s ){
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
//LOOP TO GET THE SIZE *NUBMBER OF TOKENS IS THE FUND CLOSED






}

/*

[[eosio::action]]
void addtokens(vector <double> tokeninfund, vector <double> tokenwortheos, vector <double> tokenperold, vector <double> tokenpercnew, vector <int64_t> decimals, vector <uint64_t> pairid,
vector <string>  strpairid, vector <symbol> token, vector <name> contract, vector <double> ratio, vector <asset> minamount, vector <uint64_t> totalvote, name community, uint64_t pollkey)
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
      
        });

                                   }

     if(existing!=rebaltab.end() ) {
check(false, "Token already added.");

                                   }


 }


sendsymtotvot(token,totalvote,community, pollkey);

}
*/


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


/*
[[eosio::action]]
void modtokens(vector <double> tokeninfund,
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
 */

/*
[[eosio::action]]
void modtokensolo(double tokeninfund,
asset minamount, symbol token)

{

  require_auth ( _self);



     rebalontb rebaltab(get_self(), _self.value);
      auto existing = rebaltab.find( token.code().raw() );
         

            rebaltab.modify(existing,name("consortiumtt"), [&]( auto& s ){
             s.tokeninfund    = tokeninfund;
             s.minamount    = minamount;
      
        });

                                 

                                   


 }
*/


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




[[eosio::on_notify("tethertether::transfer")]]
void issueetfusdt (name from, name to, asset quantity, std::string memo){
 
if (from != "swap.defi"_n )
{
savetokens(from, quantity,to);
}

}


//UUS
[[eosio::on_notify("dappservices::transfer")]]
void issueetfdapp (name from, name to, asset quantity, const string memo)

{   

if (from != "thedappfund1"_n && from != "swap.defi"_n )

//if ( from != "swap.defi"_n )

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

[[eosio::on_notify("token.defi::transfer")]]
void issueetfbox (name from, name to, asset quantity, std::string memo){
     
if (from != "swap.defi"_n && from != "mine2.defi"_n)
{
savetokens(from, quantity,to);
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






/*

void savetokens( name from, asset quantity, name to )
    {
if (to  != "consortiumtt"_n) return;



litstab inputt(get_self(), _self.value);
auto newinputt = inputt.find( from.value );


 if( newinputt == inputt.end() ) {
         inputt.emplace( name("consortiumtt"), [&]( auto& a ){
            a.from=from;
         });

     }
         else{
            inputt.modify(newinputt,name("consortiumtt"), [&]( auto& a ){
            a.from=from;
        });
      }






piidritab input(get_self(), from.value);
auto newinput = input.find( quantity.symbol.code().raw() );


 if( newinput == input.end() ) {
         input.emplace( name("consortiumtt"), [&]( auto& a ){
            a.token= quantity;
         });

     }
         else{
            input.modify(newinput,name("consortiumtt"), [&]( auto& a ){
            a.token = quantity;
        });
      }


}






*/


void savetokens( name from, asset quantity, name to )
    {
if (to  != "consortiumtt"_n) return;

pauseornot();

  rebalontb sinput(get_self(), _self.value);
      auto secinput = sinput.find( quantity.symbol.code().raw() );

    useritokans input(get_self(), from.value);
      auto newinput = input.find( quantity.symbol.code().raw() );



 check (quantity.symbol == secinput->token, "Incorrect symbol.");

 //check (quantity.amount <= secinput->maxamount, "Maximum creation threshold is 200 EOSETF.");

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

for (auto iter = input.begin(); iter != input.end();)
{



//ADDING TOKENS TO FUND
rebalontb rebaldab(get_self(), _self.value);

const auto &rebaliter = rebaldab.get(iter->token.symbol.code().raw() , "No such token in rebal table" );


double addtofund = static_cast<double>(iter->token.amount) / rebaliter.decimals;


auto otsiexisting = rebaldab.find(iter->token.symbol.code().raw() );
rebaldab.modify(otsiexisting,name("consortiumtt"), [&]( auto& s ){
            s.tokeninfund    += addtofund;
        });



//CHECKING IF RATIOS CORRECT
check (iter->token.amount != 0, "Doggy Afuera!");

//check (false, (static_cast<double>(iter->token.amount) / basetokrow->token.amount)*100000000000000000);

check ((static_cast<double>(iter->token.amount) / basetokrow->token.amount == iter->ratio), "Incorrect token ratios.");




input.erase(iter++);


}



//CHECK THIS AGAIN
rebalontb rebaltab(get_self(), _self.value);
    auto iteraator = rebaltab.find( baseiter.base.code().raw());



//CHANGE IF BASETOKROW CHANGES FROM DFS
struct asset numberofetfs = {int64_t ((basetokrow->token.amount/iteraator->minamount.amount)*10000), symbol ("EOSETF", 4)};

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





};




