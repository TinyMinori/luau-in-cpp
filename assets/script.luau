--!strict

print("hello starting")

function printHello (): nil
    print("[Luau] Hello World from Luau !");
end

function returnFunc (): string
    return "hello world from Luau in a return";
end

function isPositive (x: number): boolean
    return x >= 0;
end

function isEven (x: number): boolean
    return (x % 2) == 0;
end

function isHere (): boolean
    return true;
end

function zozo ()
    local a = 3;
    local b = 4;

    local sum: number = a + b

    print(string.format("Here is the sum of %d and %d: %d", a, b, sum))

    local avg: number = (a + b) / 2.0;
    print(string.format("Here is the average of %d and %d: %f", a, b, avg))
end

local i = 49;
result = isPositive(i);

print(string.format(
    "[Luau] Is %d positive ? %s", i, tostring(result)))
result = isEven(i);
print(string.format(
    "[Luau] Is %d even ? %s", i, tostring(result)))