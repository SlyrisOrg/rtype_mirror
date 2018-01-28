--
-- Created by IntelliJ IDEA.
-- User: milerius
-- Date: 22/01/18
-- Time: 20:00
-- To change this template use File | Settings | File Templates.
--

x = 1;
y = 2;

function bar()
    local obj = getEntityWithBoundingBoxComponents();
    for i, go in ipairs(obj) do
    end
    local entity = getEntity(test);
    local array = entity:getBoundingBoxComponent():getPos();
    print(array[x]);
    print(array[y]);
    entity:getBoundingBoxComponent():setPosition(300, 400);
    return array[x];
end

function foo(str)
    print(str);
    return "lol";
end

function affz()
    print('z');
end

function update_internal(id, timeSinceLastFrame)
    print("lua1");
end

testTable = {
    update = update_internal;
}