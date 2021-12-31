# Nft Gallery 🖼️
This project demostrates a simple NFT viewing gallery made with Unreal Engine 4! You can view NFTs from a given wallet address on either the Solana or Ethereum blockchains. Please view [this youtube video](https://youtu.be/XgU5Lh2sHco) for full demo.

<br>
<div align="center">
<img src="demo.gif" width="700" height="400" />
</div>
<br>

 - The main functionality is done in the `Content/NftDataGetter` blueprint. 
 - I also utilize a custom blueprint function made with C++ to make an Http request (based off this [tutorial](https://www.tomlooman.com/unreal-engine-async-blueprint-http-json/)). You can see the C++ functionality in [LZAsyncAction_RequestHttpMessage.cpp](https://github.com/jjohnson5253/nftGallery/blob/main/Source/web3Bp/LZAsyncAction_RequestHttpMessage.cpp). 
 - To get the NFT data for a given wallet address, I used NftPort's API for Ethereum and AllArt's API for Solana. 
 - Credit should also be given to this [youtube video](https://www.youtube.com/watch?v=u6ns9U9sN5s) for showing me how to change dynamic textures to images from a URL.