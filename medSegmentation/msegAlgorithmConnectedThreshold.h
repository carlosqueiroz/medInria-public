
#ifndef _msegAlgorithmConnectedThreshold_h_
#define _msegAlgorithmConnectedThreshold_h_

#include "msegAlgorithmGeneric.h"

#include <QVector3D>

class dtkAbstractData;

namespace mseg {

/** Connected Region Growing segmentation algorithm */
class AlgorithmConnectedThreshold : public AlgorithmGeneric {
public:
    AlgorithmConnectedThreshold();
    ~AlgorithmConnectedThreshold();

    QVector3D seedPoint() const { return m_seedPoint; }
    void setSeedPoint(const QVector3D & val) { m_seedPoint = val; }

    double lowThreshold() const { return m_lowThreshold; }
    void setLowThreshold(double val) { m_lowThreshold = val; }
    double highThreshold() const { return m_highThreshold; }
    void setHighThreshold(double val) { m_highThreshold = val; }

    void run();

protected:
    template < typename TPixel, unsigned int VDimension > 
        void doFilter( dtkAbstractData * inData );

private:
    QVector3D m_seedPoint;

    double m_lowThreshold;
    double m_highThreshold;

};

} // namespace mseg

#endif // _msegAlgorithmConnectedThreshold_h_
