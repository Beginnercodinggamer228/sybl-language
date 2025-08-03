# SyBL - Symbol Programming Language

Simple interpreted programming language with unique symbolic syntax.

## 📥 Download & Install

### Windows
1. **Download:** [sybl-windows.zip](https://github.com/Beginnercodinggamer228/sybl-language/blob/main/sybl-windows.zip)
2. **Extract** to any folder (e.g., `C:\SyBL\`)
3. **Install:** Double-click `install.reg` 
4. **Done!** Now `.syb` files run with double-click

### VS Code Extension
- Search **"SyBL Symbol Programming Language"** in VS Code Extensions
- Or install from [VS Code Marketplace](https://marketplace.visualstudio.com/items?itemName=GenkaMistikToCoder.sybl-language)

## 🚀 Quick Start

Create `hello.syb`:
```sybl
;: My first SyBL program
>_ "Hello, World!"

name =#= "Developer"
age =$= 25

>_ "Hello, $%name%!"
>_ "You are $%age% years old"
```

**Double-click `hello.syb` to run!** ✨

## 📖 Syntax

### Variables
```sybl
number =$= 42              ;: Integer
pi =$.$= 3.14             ;: Float
text =#= "Hello"          ;: String
flag =&= +                ;: Boolean (+ = true, - = false)
```

### Output
```sybl
>_ "Simple text"
>_ "Value: $%number%"      ;: Variable in text
```

### Math
```sybl
a =$= 10
b =$= 5
sum =$= a + b             ;: +, -, *, /
>_ "Result: $%sum%"
```

### Conditions
```sybl
age =$= 18
?(age >= 18)?
>_ "Adult"
?end?
```

### Loops
```sybl
@(i < 5)@
>_ "Count: $%i%"
@end@
```

## 📁 Examples

- `hello.syb` - Hello World
- `example.syb` - Math, conditions, loops
- `main.syb` - All language features

## 🛠️ Usage

- **Run:** Double-click `.syb` files
- **Edit:** Use VS Code with SyBL extension
- **Command line:** `sybl program.syb`

---

**SyBL works like Python - just double-click to run!** 🎉
