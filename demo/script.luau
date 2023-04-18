--! strict

print("Hello World from Luau !");

function ispositive(x : number) : string
    if x > 0 then
        return "yes"
    else
        return "no"
    end
end

function iseven(x: number): string
    if (x % 2 == 0) then
        return "yes"
    else
        return "no"
    end
end

local i = 49;
local result : string
result = ispositive(i);

print(string.format(
    "Is %d positive ? %s", i, result))

result = iseven(i);
print(string.format(
    "Is %d even ? %s", i, result))