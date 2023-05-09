print("hello starting")

function hello (): nil
    print("[Luau] Hello World from Luau !");
end

function returnFunc (): string
    return "hello world from Luau in a return";
end

function ispositive (x: number): boolean
    return x > 0;
end

function iseven (x: number): boolean
    return (x % 2 == 0)
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
result = ispositive(i);

print(string.format(
    "[Luau] Is %d positive ? %s", i, tostring(result)))

result = iseven(i);
print(string.format(
    "[Luau] Is %d even ? %s\n", i, tostring(result)))