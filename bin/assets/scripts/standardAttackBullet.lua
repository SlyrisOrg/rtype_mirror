--
-- Created by IntelliJ IDEA.
-- User: king
-- Date: 1/26/18
-- Time: 7:58 PM
-- To change this template use File | Settings | File Templates.
--

posX = 0;
posY = 0;

targetX = 0;
targetY = 0;

fireRate = 3;

winWidth = 1920;
winHeight = 1080;

function internal_collide(meID, collideID)
    local ettMe = getEntity(meID);
    local ettCol = getEntity(collideID);


    if ettCol:hasEnemyComponent() and ettMe:hasAlliedComponent() then
    end

    if ettCol:hasAlliedComponent() and ettMe:hasEnemyComponent() then
    end
end

function internal_update(meID, timeSinceLastFrame)
end

standardAttackBulletTable = {
    onCollide = internal_collide;
    update = internal_update;
}