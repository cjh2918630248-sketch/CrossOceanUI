#ifndef VELOCITYINPUTINTERFACE_H
#define VELOCITYINPUTINTERFACE_H

class VelocityInputInterface
{
public:
    virtual ~VelocityInputInterface() = default;
    virtual void setSpeed(double speed) = 0;
    virtual double speed() const = 0;
};

#endif // VELOCITYINPUTINTERFACE_H
