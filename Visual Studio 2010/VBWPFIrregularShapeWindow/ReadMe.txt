================================================================================
       Windows Ӧ��: VBWPFIrregularShapeWindow����                     
===============================================================================
/////////////////////////////////////////////////////////////////////////////
��Ҫ:
������Ӳ���������ΪWPF���û����洴��һ����״������Ĵ��塣
��������еĲ����������״�����ˣ����Σ�Բ�Σ���������ͼƬΪ��������״�ȵȡ�
��ÿһ��WPF���������ǿ�������һ����ҵ�������˵�����󻯣���С����ر����ֲ��������⻹Ϊ��Щ��
����Ĵ����������ק��Ϊ��

////////////////////////////////////////////////////////////////////////////////
��ʾ:

����1. �� VS2010�д��������Ŀ. 

����2. �����ʹ������PNG��ʽ��ͼƬ�ļ�����transparentPic.png ����ʹ��Ĭ��ͼƬ��

����3. ���� VBWPFIrregularShapeWindow.exe.

����4. �ֱ��� "��Բ�δ���","Բ�Ǵ���","�����δ���","�Ի�����","ͼƬ��������" �����ǻ���ʾ��Ӧ
	  �Ĳ������塣 


/////////////////////////////////////////////////////////////////////////////
�����߼�:

1. ÿһ������������һ����������࣬����Ϊ���������¼�����������һ��·�����
2. ���ǿ���ʹ�����ű仯���Ŵ���С����ĳߴ硣�������������෴�Ķ��������Ե���ͬ��������Ӱ�����ʱһ����Ч��һ����Ч��

3. �����ʹ��һ���˵�����Ӱ��һ��������ô��Ӧ�����ò˵������Ŀ�귽���Ͱ�ť������˵��������Ч��

4. ��������ṩ��һ���ֳɵ��������ж�CommandBinding�Ķ�������canExecute�������Խ��ܽ�������١�

5. �������ʹ�����¼��������͹�����壬���磺http://msdn.microsoft.com/en-us/library/ms745683.aspx#Y3189,
   ������Ѿ�����������������ᵽ�ģ���ᷢ����һ�ִ��������Ǿ����¼������������ִ����������¼��ķ�����ʼһ��������Ϊ��
   ���磬�¼�������������ϸ˵��������Ƿ������˰�ť����Բ��ͼƬ���߶����Ե�ֵ��0.2�����Զ��䵽260��
   �����Ӳ˵������ƿ�ʱ����������1���ڱ�ص�ԭʼֵ��ע���������������뿪ʱ�������ö���ֵ�ġ�
   ������Ϊ�����ǿ��Ա���ԭʼֵ�Ĺ켣�ġ���ֻ��Ҫʹ������Ĵ��룬�����е�Ŀ�������滻�ɰ�ť��·����
   ��Բ����ͼƬ���ɡ�
     <Window.Resources>
        <Style TargetType="Path">
            <Setter Property="Opacity" Value="0.5" />
            <Style.Triggers>

                <EventTrigger RoutedEvent="Mouse.MouseEnter">
                    <EventTrigger.Actions>
                        <BeginStoryboard>
                            <Storyboard>
                                <DoubleAnimation
                  Duration="0:0:0.2"
                  Storyboard.TargetProperty="Height"
                  To="260"  />
                            </Storyboard>
                        </BeginStoryboard>
                    </EventTrigger.Actions>
                </EventTrigger>
                <EventTrigger RoutedEvent="Mouse.MouseLeave">
                    <EventTrigger.Actions>
                        <BeginStoryboard>
                            <Storyboard>
                                <DoubleAnimation
                  Duration="0:0:1"
                  Storyboard.TargetProperty="Height"  />
                            </Storyboard>
                        </BeginStoryboard>
                    </EventTrigger.Actions>
                </EventTrigger>
            </Style.Triggers>

        </Style>

    </Window.Resources>

	��Ȼ������ҪΪ��ť��·������Բ��ͼƬ����߶����Բ�����������ֵ�����磺
	<Path Height="250" Name="Triangle" Stroke="Black" StrokeThickness="2" Grid.ColumnSpan="10" Grid.RowSpan="4">

6.Ϊ�����Ӵ��ڲ����������غϣ�����������ʹ�õĴ������£�

	ellipseWindow.Left = this.Left + this.Width + width;
	ellipseWindow.Top = this.Top + this.Height + height;

/////////////////////////////////////////////////////////////////////////////
�ο�����:

CommandBinding �� http://msdn.microsoft.com/zh-cn/library/system.windows.input.commandbinding.aspx
RoutedUICommand �� http://msdn.microsoft.com/zh-cn/library/ms752308.aspx
ScaleTransform ��http://msdn.microsoft.com/zh-cn/library/system.windows.media.scaletransform.aspx
Window.WindowState ����  http://msdn.microsoft.com/zh-cn/library/system.windows.window.windowstate.aspx
��ʽ���ú�ģ�廯 http://msdn.microsoft.com/en-us/library/ms745683.aspx#Y3189
/////////////////////////////////////////////////////////////////////////////



