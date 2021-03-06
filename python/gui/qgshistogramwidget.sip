
/** \ingroup gui
 * \class QgsHistogramWidget
 * \brief Graphical histogram for displaying distributions of field values.
 *
 * \note Added in version 2.9
 */

class QgsHistogramWidget : QWidget
{
%TypeHeaderCode
#include <qgshistogramwidget.h>
%End

 public:

    /** QgsHistogramWidget constructor. If layer and fieldOrExp are specified then the histogram
     * will be initially populated with the corresponding values.
     * @param parent parent widget
     * @param layer source vector layer
     * @param fieldOrExp field name or expression string
     */
    QgsHistogramWidget( QWidget *parent /TransferThis/ = 0, QgsVectorLayer* layer = 0, const QString& fieldOrExp = QString() );
    
    ~QgsHistogramWidget();

    /** Returns the layer currently associated with the widget.
     * @see setLayer
     * @see sourceFieldExp
     */
    QgsVectorLayer* layer();

    /** Returns the source field name or expression used to calculate values displayed
     * in the histogram.
     * @see setSourceFieldExp
     * @see layer
     */
    QString sourceFieldExp() const;

    /** Sets the pen to use when drawing histogram bars. If set to Qt::NoPen then the
     * pen will be automatically calculated. If ranges have been set using @link setGraduatedRanges @endlink
     * then the pen and brush will have no effect.
     * @param pen histogram pen
     * @see pen
     * @see setBrush
     */
    void setPen( const QPen& pen );

    /** Returns the pen used when drawing histogram bars.
     * @see setPen
     * @see brush
     */
    QPen pen() const;

    /** Sets the brush used for drawing histogram bars. If ranges have been set using @link setGraduatedRanges @endlink
     * then the pen and brush will have no effect.
     * @param brush histogram brush
     * @see brush
     * @see setPen
     */
    void setBrush( const QBrush& brush );

    /** Returns the brush used when drawing histogram bars.
     * @see setBrush
     * @see pen
     */
    QBrush brush() const;

    /** Sets the graduated ranges associated with the histogram. If set, the ranges will be used to colour the histogram
     * bars and for showing vertical dividers at the histogram breaks.
     * @param ranges graduated range list
     */
    void setGraduatedRanges( const QgsRangeList& ranges );

    /** Returns the graduated ranges associated with the histogram. If set, the ranges will be used to colour the histogram
     * bars and for showing vertical dividers at the histogram breaks.
     * @returns graduated range list
     * @see setGraduatedRanges
     */
    QgsRangeList graduatedRanges() const;

  public slots:

    /** Triggers a refresh of the histogram when the widget is next repainted.
     */
    void refreshHistogram();

    /** Triggers a refresh and immediate redraw of the histogram.
     */
    void refreshAndRedraw();

    /** Sets the vector layer associated with the histogram.
     * @param layer source vector layer
     * @see setSourceFieldExp
     */
    void setLayer( QgsVectorLayer* layer );

    /** Sets the source field or expression to use for values in the histogram.
     * @param fieldOrExp field name or expression string
     * @see setLayer
     */
    void setSourceFieldExp( const QString& fieldOrExp );

  protected:

    /** Updates and redraws the histogram.
     */
    virtual void drawHistogram();

    virtual void paintEvent( QPaintEvent * event );
};
