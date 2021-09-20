# Eve Programming Language

[![made-with-C](https://img.shields.io/badge/Made%20with-C-1f425f.svg)](https://en.wikipedia.org/wiki/C_(programming_language)) [![GPLv3 license](https://img.shields.io/badge/License-GPLv3-blue.svg)](http://perso.crans.org/besson/LICENSE.html)

> Contributors

[![Contributors Display](https://badges.pufler.dev/contributors/ibukiyoshidaa/eve?size=50&padding=5&bots=true)](https://github.com/ibukiyoshidaa/eve)

> How to use Eve

```
$ sudo make install
```

```
$ eve <filename>.eve
```

> Hello World! in Eve
```swift
eve["Hello World!"]:

eve["こんにちは世界！"]:

```

> Function
```swift
func main[x, y] {
    eve[x, y]:
}:

main["Hello", "World"]:
```

> List
```swift
list Animals [
    "dog",
    "cat",
    "penguin",
    "girafe",
    "snake"
]:

eve[Animals->0]:
```

<a href="https://eveofficial.herokuapp.com/docs">how to use | syntax<a>
