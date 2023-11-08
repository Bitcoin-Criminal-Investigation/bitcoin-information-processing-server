# BTDS (Bitcoin Tracking Detecting Solution)

범죄 수사 지원을 목적으로, 비트코인 거래 흐름을 자동으로 추적하고 시각화하는 솔루션

<aside>
💡 BTDS는 수사 기관을 위해 개발된 솔루션으로, 복잡한 비트코인 거래를 그래프 형태로 시각화하여 범죄 수사의 효율성을 높입니다. 이 솔루션은 자동 추적 기능을 통해 거래의 흐름을 빠르게 파악하고 분석할 수 있도록 설계되었습니다.
</aside>

<br/>
<br/>
<br/>

> Web-Server : [https://github.com/Bitcoin-Criminal-Investigation/bitcoin-tracking-detecting-solution-web](https://github.com/Bitcoin-Criminal-Investigation/bitcoin-tracking-detecting-solution-web)
>
> Core-Server : [https://github.com/Bitcoin-Criminal-Investigation/bitcoin-information-processing-server](https://github.com/Bitcoin-Criminal-Investigation/bitcoin-information-processing-server)
>

<br/>

---

# bitcoin-information-processing-server

## Getting Started

### Prerequisite installer

Install mongocxx, blocksci, cpprestsdk, nlohmann/json

> https://mongocxx.org/mongocxx-v3/installation/linux
>
> https://github.com/citp/BlockSci
>
> https://github.com/microsoft/cpprestdk
>
> https://github.com/nlohmann/json

### Setting Environment

```bash
export MONGO_URI=mongodb://username:password@localhost:27017
export BLOCKSCI_SETTING=BlockSCI-setting-file-location
export SERVER_URL=http://0.0.0.0:port # 로컬에서만 접속 허용 시 127.0.0.1
```

### Compile and start

```Bash
> make
> ./core-server
```
