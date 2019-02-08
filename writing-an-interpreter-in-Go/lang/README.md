# yolang
一个简单的解释型语言，只是个玩具

```
wangpingdeMacBook-Air:yolang wangping$ ./main
Hello wangping, welcome to golang world>>
>>let a = 2
>>let b = 3
>>a + b
5
>>let array = [1, 2, a+b]
>>array
[1, 2, 5]
>>array[1]
2
>>array[1+0]
2
>>array[1000]
null
>>len(array)
3
>>if (true) {return "truexx"}
truexx
>>if (false) {return "falsexx"}
null
>>let dict = {"a": true}
>>dict["a"]
true
>>let func = fn(x){return x+2}
>>func(3)
5
>>

```
