#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UHealthBarWidget::UpdateHealthBar(float HealthPercent)
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetPercent(HealthPercent);
    }
}
