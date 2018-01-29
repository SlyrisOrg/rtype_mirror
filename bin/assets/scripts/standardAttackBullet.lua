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

shooter = {}

function internal_collide(meID, collideID)
    print("bonjour");
    local ettMe = getEntity(meID);
    local ettCol = getEntity(collideID);
    local ettShooter = getEntity(shooter[meID]);


    if ettCol:hasEnemyComponent() and ettMe:hasAlliedComponent() then
        ettCol:getStatComponent().hp = ettCol:getStatComponent().hp - ettShooter:getStatComponent().attack;
        mark(meID);
    end

    if ettCol:hasAlliedComponent() and ettMe:hasEnemyComponent() then
        ettCol:getStatComponent().hp = ettCol:getStatComponent().hp - ettShooter:getStatComponent().attack;
        mark(meID);
    end
    print("neat");
end

function internal_update(meID, timeSinceLastFrame)
end

function internal_init(meID, shooterID)
    shooter[meID] = shooterID;
end

standardAttackBulletTable = {
    onCollide = internal_collide;
    update = internal_update;
    init = internal_init;
}