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

local i = 49;
local result : string
result = ispositive(i);

print(string.format(
    "[Luau] Is %d positive ? %s", i, tostring(result)))

result = iseven(i);
print(string.format(
    "[Luau] Is %d even ? %s\n", i, tostring(result)))