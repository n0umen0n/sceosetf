<h1> <img src="https://serving.photos.photobox.com/80569519fc9a78ab8e2a713de35656f41e0cda82a74640a81c611f8ebe55fe267d3f5415.jpg" width="55" height="55"> CETF - Decentralized Exchange Traded Funds </h1>


Welcome to the CETF's smart contracts repository! 


<p>Contract is deployed to the following account on the EOS mainnet (currently being tested on mainnet on consortiumtt account):
        <ul>
        <li><b>cet.f</b>   </li>   </ul></p>
        <hr></hr>

<h3>Contract - cet.f </h3>

Code contains functions that enable to create and manage ETF consisting of eosio tokens (ETF itself is also standard eosio token).

Let's dive deeper into how the CREATION and MANAGEMENT exactly works. All the functions can be found in <b>eosetf.cpp</b> file.
        <hr></hr>

1. CREATION

In order to create ETF user has to send to cet.f tokens that are currently in the ETF (which tokens are in the fund could be seen in rebalon table, scope contract name - cet.f). The amount of tokens that are sent in, have to be EXACTLY in correct proportions in relation to each other. 

Example, let's assume to create one ETF we need to send in three tokens:

<br><b>100.0000 DAPP</b><br/>
<br><b>25.0000 VIG</b> <br/>
<br><b>10.0000 EMT</b> <br/>


To check whether the amounts are correct, we read the current ratios (proportions between tokens) in the rebalon table.

Ratios are calculated in the following way:

1. From all the current ETF tokens, one base token is chosen. 
2. All the other tokens are divided by the base token.

Let's take EMT as base token and calculate the ratios.

<br>100 DAPP / 10 EMT = 10<br/>
<br>25 VIG / 10 EMT = 2.5<br/>
<br>10 EMT / 10 EMT = 1<br/>



If incoming tokens to cet.f do not produce those ratios, no ETF will be created/issued.

<br>Example, if user sends in:<br/>
<br>99.0000 DAPP<br/>
<br>25.0000 VIG<br/>
<br>10.0000 EMT<br/>

<br>99 DAPP / 10 EMT = 9.9 -> INCORRECT has to be 10<br/>
<br>25 VIG / 10 EMT = 2.5 -> CORRECT<br/>
<br>10 EMT / 10 EMT = 1 -> CORRECT<br/>

No ETF is issued. 
       
<hr></hr>


2. MANAGEMENT 

Currently cet.f msiggers can whitelist fund managers. 

Fund managers are able to use 100 credits (100%). To vote and pick which tokens should be in the ETF.

When 2/3 fund managers have voted, anybody can trigger rebalancing function.

rebalance function triggers sell/buy orders through Defibox, based on how fund managers collectively voted.

Rebalance function calculates new ratios mentioned in the previous 1.CREATION section.

Fund can be rebalanced as many times as fund managers wish, as long as 2/3 have voted. 

<hr></hr>

2. TOKENOMICS

Currently code contains functions enabling to claim ETF creation fees. 

<br>Functions:<br/>

<br>getdiv - claims the fees generated<br/>

<br>stakecetf - stakes CETF token to be eligible to claim ETF fees<br/>

<br>unstkcetf - unstakes CETF<br/>
