#include<iostream>
#include<cmath>
#include<cstring>
#include<sstream>
#include<fstream>
using namespace std;
namespace shuru
{
    template < typename T > std::string to_string( const T& n )  //to_string()  
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}
template<typename T>
struct playnode
{
	int id;
	T key;
	bool operator <= (const playnode &x) const//id��������idһ���Ļ�key�������� 
	{
		if(id!=x.id)
		{
			if(id<=x.id)
			    return true;
			else
			    return false;    
		}
		else
		{
			if(key<=x.key)
			    return true;
			else
			    return false;     
		}
	}
};
template<class T>
class LoserTree
{
	public: 
		LoserTree(T *theplayer,int then)//��ʼ�� 
		{
			Build(theplayer,then);
		}
		void Build(T *theplayer,int then);//���� 
		void play(int p,int l,int r);//������ 
		void replay(int x);//���������� 
		int loser()//�������� 
		{
			if(n==0) 
				return 0;
			return tree[1];
		}
	private:
		int Buffer;//������ 
		int n;//Ҫ������������� 
		int lowExt;//��ʾ���һ���ж��ٵ� 
		int offset;//��ʾһ��ƫ���� 
		int *tree;//�洢���ŵĵ�������еı�� 
		T* player;//�������˳��/���еĵ�ǰ����� 
};
template<class T>
void LoserTree<T>::Build(T * theplayer,int then)
{
	player=theplayer;//�������˳��/���еĵ�ǰ�����
	n=then;//�����Ԫ������ 
	tree=new int [n+1];//�������ڵ� 
	int s=log(n-1)/log(2);//��������log��2Ϊ��n-1��ֵ 
	s=pow(2,s);//���������Ľڵ��� 
	lowExt=2*(n-s);//ʣ��ľ������һ��Ľ���� 
	offset=2*s-1;//ƫ���� 
	int i=0; 
	for(int i=2;i<=lowExt;i=i+2)//�������һ��
		play(i+offset>>1,i-1,i);//����һ����Ƚ� 
    if(n%2==1)//�����������������������ֵܵĺϲ� 
	{
		play(n/2,tree[n-1],lowExt+1); 
		i=lowExt+3;
	} 
	else 
		i=lowExt+2;
	for(;i<=n;i=i+2)// ��������һ�� 
		play((i-lowExt+n-1)/2,i-1,i); 
}
template<class T>
void LoserTree<T>::play(int p,int l,int r)//p�ǵ�ǰ���ڵ㣨�����ߣ���l�Ǵ��������ߵ���ң�rΪ��������ұߵ���� 
{
	if(player[l]<=player[r])//����������ߣ�����Ǹ�������¼������Ϊ��Ȼ����һ��Ƚ� 
	    tree[p]=l;
	else 
	    tree[p]=r;
	//pΪż����ֻ����һ�Σ���ֻ���������һ�㣬��ʱ������Ҷ������У�ֱ������    
	while((p%2==1)&&p>1)//�ұߵ���Ҹպ���ż�� 
	{
		if(player[tree[p-1]]<=player[tree[p]])//ͬ�� 
		{
			tree[p/2]=tree[p-1];
		}
		else 
		    tree[p/2]=tree[p];
		p=p/2; 
	}
}
template<class T>
void LoserTree<T>::replay(int k)//������������kΪ��ҵ�ֵ 
{
	if(k<0||k>n) 
	return ;
	int now,l,r;//��ҵ�ֵ������ң������ 
	if(k<=lowExt)//����ڵײ� 
	{
		now=offset+k>>1;  //father�ڵ�ı�� 
		l=2*now-offset; 
		r=l+1;
	}
	else//����ڵ����ڶ� 
	{
		now=(k-lowExt+n-1)/2;
		if(2*now==n-1)//����ķ��� 
		{
			l=tree[2*now];
			r=k;
		} 
		else 
		{
			l=2*now-(n-lowExt-1);
			r=l+1;
		}
	}
	if(player[l]<=player[r])//ͬ�� 
	    tree[now]=l;
	else 
	    tree[now]=r;
	if(now==n-1&&(n%2==1))
	{
		now>>=1;
		if(player[tree[n-1]]<=player[lowExt+1])
			tree[now/2]=tree[n-1];
		else 
		    tree[now/2]=lowExt+1;
	}
	now>>=1;
	for(;now>=1;now>>=1)
	{
		if(player[tree[now*2]]<=player[tree[now*2+1]])
		 	tree[now]=tree[now*2];
		else 
		    tree[now]=tree[now*2+1];
	}
}
template<class T>
class ExternalSort
{
	public: 
		void Kway();  //�ϲ� 
		void Solve();
		void replay();
		int count;
	private:
		playnode<T> *player;
		int Buffer;
		int n;
		int maxid;
		int *cnt;
};
template<class T>
void ExternalSort<T>::Solve()
{
	cin>>n;//������ַ����� 
	Buffer=10;//���������� 
	count=0;//��¼�������� 
	cnt = new int [n+1];//��¼���� 
	playnode<int> *player=new playnode<T>[Buffer+1];
	for(int i=1;i<=Buffer;i++)
	{
		cin>>player[i].key;//����ÿ���ַ� 
		player[i].id=1;//��ʼ�� 
		count++;//��������++ 
	}
	LoserTree<playnode<T>> tree(player,Buffer);//������ 
	maxid=0;//���Ľ���ʼ�� 
	int now=Buffer+1;
	for(int i=1;i<=n;i++) 
	    cnt[i]=0;//��ʼ�� 
	for(int i=Buffer+1;i<=n;i++)//�ڻ�����֮����� 
	{
		int loser = tree.loser();//�������� 
		int id=player[loser].id;//ȷ����� 
		cnt[id]++;//λ�μ�һ 
		maxid=max(maxid,id); 
		string file=shuru::to_string(id)+".in";
		ofstream fout(file.c_str(), ios::app);
		if(cnt[id]>1)//��һ�������������֣�����ÿ�����������ո�+���֣������ʹﵽ�����һ������û�пո��Ҫ���� 
		    fout<<' ';
		fout<<player[loser].key;
		count++;//��������+1 
		fout.close();
		int key;
		cin>>key;//�ڻ�����֮������Ҫ������ 
		    count++;//��������+1 
		if(key<player[loser].key)//ȡ��ŵ����ֵ����ֵ 
		    player[loser].id++;
		player[loser].key=key; 
		tree.replay(loser);
	}
	for(int i=1;i<=Buffer;i++)//�������ڲ�Ҳ��һ���� 
	{
		int loser = tree.loser();
		int id=player[loser].id;
		cnt[id]++;
		maxid=max(maxid,id);
		string file=shuru::to_string(id)+".in";
		ofstream fout(file.c_str(), ios::app);
		if(cnt[id]>1) 
		    fout<<' ';
		fout<<player[loser].key;
		fout.close();
		player[loser].key=0;//������Ϊ��ʼ�� 
		player[loser].id=n+1;
		tree.replay(loser);
	}
}
template<class T>
void ExternalSort<T>::Kway()
{
	int div=Buffer/maxid,mod=Buffer%maxid;//�����̺����� 
	player = new playnode<int>[Buffer+1];
	ifstream  *file=new ifstream[maxid+1];
	for(int i=1;i<=maxid;i++) 
	{
		string tmp=shuru::to_string(i)+".in";
		const char *name=tmp.c_str();
		file[i].open(tmp.c_str());
	}
	int x;
	for(int i=1;i<=maxid;i++)//�������� 
	{	
		if(i<=mod) 
		    x=1;
		else 
		    x=0;
		cnt[i]=min(cnt[i],div+x);
	}
	for(int i=1;i<=Buffer;i++)//��ʼ�� 
	    player[i].id=2;
	int tot=0;//�������� 
	int *belong = new int [n+1];//�������ʾ���� 
	for(int i=1;i<=maxid;i++)
	{
		for(int j=1;j<=cnt[i];j++)
		{
			file[i]>>player[tot+j].key;
			player[tot+j].id=1;
			belong[tot+j]=i;
		}
		tot=tot+cnt[i];
	}
	LoserTree<playnode<T> > tree(player,Buffer);
	for(int i=1;i<=n;i++)//ͬ�� 
	{
		int loser = tree.loser();
		int id=belong[loser];
		string out="out.txt";
		ofstream fout(out.c_str(), ios::app);
		fout<<player[loser].key<<" ";
		cnt[id]++;
		count++;
		if(file[id].peek()!=EOF)//û����������룬�����ȷ����ţ�peek��Ϊ���������� 
		    file[id]>>player[loser].key;
		else 
		    player[loser].id=2;
		tree.replay(loser);
	}
	return ;
}
int main()
{ 
	freopen("in.txt","r",stdin);//�����ļ�����һ�����г���n����һ�����С�
	ExternalSort<int> A;
	A.Solve();
	A.Kway();
	cout<<"�˴�ִ�еĲ���Ϊ���Σ�:"<<A.count;
	return 0; 
}

