using System;
using System.Collections.Generic;

namespace UVEngine
{
    ///// <summary>
    ///// ΪӦ�ó���ʵ�� IServiceProvider��������ͨ�� App.Services
    ///// ���Թ����������� ContentManagers ��������Ҫ���� IServiceProvider �����͡�
    ///// </summary>
    //public class AppServiceProvider : IServiceProvider
    //{
    //    // ������ķ�������ӳ��
    //    private readonly Dictionary<Type, object> services = new Dictionary<Type, object>();

    //    /// <summary>
    //    /// ���·�����ӵ������ṩ����
    //    /// </summary>
    //    /// &lt;param name="serviceType"&gt;Ҫ��ӵķ������͡�</param>
    //    /// &lt;param name="service"&gt;���������</param>
    //    public void AddService(Type serviceType, object service)
    //    {
    //        // ��֤����
    //        if (serviceType == null)
    //            throw new ArgumentNullException("serviceType");
    //        if (service == null)
    //            throw new ArgumentNullException("service");
    //        if (!serviceType.IsAssignableFrom(service.GetType()))
    //            throw new ArgumentException("service does not match the specified serviceType");

    //        // ��������ӵ��ʵ�
    //        services.Add(serviceType, service);
    //    }

    //    /// <summary>
    //    /// �ӷ����ṩ�����ȡ����
    //    /// </summary>
    //    /// &lt;param name="serviceType"&gt;Ҫ�����ķ������͡�</param>
    //    /// <returns>Ϊָ������ע��ķ������</returns>
    //    public object GetService(Type serviceType)
    //    {
    //        // ��֤����
    //        if (serviceType == null)
    //            throw new ArgumentNullException("serviceType");

    //        // ���ֵ��м�������
    //        return services[serviceType];
    //    }

    //    /// <summary>
    //    /// �ӷ����ṩ����ɾ������
    //    /// </summary>
    //    /// &lt;param name="serviceType"&gt;Ҫɾ���ķ������͡�</param>
    //    public void RemoveService(Type serviceType)
    //    {
    //        // ��֤����
    //        if (serviceType == null)
    //            throw new ArgumentNullException("serviceType");

    //        // �Ӵʵ���ɾ������
    //        services.Remove(serviceType);
    //    }
    //}
}
